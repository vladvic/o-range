/***************************************************
 * LuaScriptProvider.hpp
 * Created on Tue, 24 Feb 2026 21:17:45 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <boost/asio/awaitable.hpp>
#include <boost/asio/executor_work_guard.hpp>
#include <boost/asio/io_context.hpp>
#include <memory>
#include <string>
#include <thread>
#include <unordered_map>

#include <R-Range/ScriptProvider.hpp>

class ObjectArena;
class LuaEnvironment;
struct lua_State;

class LuaScriptProvider : public ScriptProvider
{
public:
  LuaScriptProvider();
  ~LuaScriptProvider() override;

  boost::asio::awaitable<std::shared_ptr<Script>> asyncGet(
    const ScriptKey& key) override;

  lua_State* envFor(ObjectArena& arena);
  void forget(ObjectArena* arena);

private:
  std::string loadScriptSource(const ScriptKey& key);

  boost::asio::io_context m_ioct;
  boost::asio::executor_work_guard<boost::asio::io_context::executor_type>
    m_workGuard;
  std::thread m_thread;

  std::unordered_map<ObjectArena*, std::weak_ptr<LuaEnvironment>> m_envs;
};
