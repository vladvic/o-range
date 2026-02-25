/***************************************************
 * LuaScriptProvider.cpp
 * Created on Tue, 24 Feb 2026 21:17:45 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "LuaScriptProvider.hpp"

#include <boost/asio/dispatch.hpp>
#include <boost/asio/use_awaitable.hpp>

#include <D2L/Lua/Bindings.hpp>
#include <D2L/LuaScript.hpp>
#include <R-Range/Object.hpp>
#include <R-Range/ObjectArena.hpp>
#include <S2B/CommandBus.hpp>
#include <lua.hpp>

class LuaEnvironment : public Object
{
public:
  LuaEnvironment(LuaScriptProvider& owner, ObjectArena& arena)
    : m_owner(owner)
    , m_arena(&arena)
  {
    m_state = luaL_newstate();
    if (m_state) {
      luaL_openlibs(m_state);
      D2L::Lua::register_types(m_state);
      D2L::Lua::set_globals(m_state, arena, CommandBus::instance());
    }
  }

  ~LuaEnvironment() override
  {
    if (m_state) {
      lua_close(m_state);
      m_state = nullptr;
    }
    m_owner.forget(m_arena);
  }

  lua_State* state() const { return m_state; }

private:
  LuaScriptProvider& m_owner;
  ObjectArena* m_arena;
  lua_State* m_state{ nullptr };
};

LuaScriptProvider::LuaScriptProvider()
  : m_workGuard(boost::asio::make_work_guard(m_ioct))
  , m_thread([this]() { m_ioct.run(); })
{
}

LuaScriptProvider::~LuaScriptProvider()
{
  m_workGuard.reset();
  m_ioct.stop();
  if (m_thread.joinable()) {
    m_thread.join();
  }
}

boost::asio::awaitable<std::shared_ptr<Script>> LuaScriptProvider::asyncGet(
  const ScriptKey& key)
{
  co_await boost::asio::dispatch(m_ioct, boost::asio::use_awaitable);

  auto source = loadScriptSource(key);
  if (source.empty()) {
    co_return nullptr;
  }

  co_return std::make_shared<LuaScript>(*this, source);
}

std::string LuaScriptProvider::loadScriptSource(const ScriptKey& key)
{
  (void)key;
  return {};
}

lua_State* LuaScriptProvider::envFor(ObjectArena& arena)
{
  auto it = m_envs.find(&arena);
  if (it != m_envs.end()) {
    if (auto env = it->second.lock()) {
      return env->state();
    }
  }

  auto env = std::make_shared<LuaEnvironment>(*this, arena);
  if (!env->state()) {
    return nullptr;
  }
  arena.add(env);
  m_envs[&arena] = env;
  return env->state();
}

void LuaScriptProvider::forget(ObjectArena* arena)
{
  if (!arena) {
    return;
  }
  m_envs.erase(arena);
}
