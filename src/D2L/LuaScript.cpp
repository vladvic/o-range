/***************************************************
 * LuaScript.cpp
 * Created on Tue, 24 Feb 2026 20:48:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "LuaScript.hpp"

#include <D2L/Lua/Bindings.hpp>
#include <D2L/LuaScriptProvider.hpp>
#include <Logger/Logger.hpp>
#include <S2B/CommandBus.hpp>
#include <S2B/SignalCommand.hpp>
#include <fstream>
#include <lua.hpp>
#include <sstream>

namespace {

std::string loadScriptSource(const std::string& script)
{
  std::ifstream file(script);
  if (!file.is_open()) {
    return script;
  }

  std::ostringstream buffer;
  buffer << file.rdbuf();
  return buffer.str();
}

bool loadAndRun(lua_State* state, const std::string& source)
{
  if (luaL_loadbuffer(state, source.c_str(), source.size(), "LuaScript") !=
      LUA_OK)
  {
    const char* msg = lua_tostring(state, -1);
    LOG_ERROR("Lua load error: {}", msg ? msg : "unknown").show();
    lua_pop(state, 1);
    return false;
  }

  if (lua_pcall(state, 0, LUA_MULTRET, 0) != LUA_OK) {
    const char* msg = lua_tostring(state, -1);
    LOG_ERROR("Lua runtime error: {}", msg ? msg : "unknown").show();
    lua_pop(state, 1);
    return false;
  }

  return true;
}

} // namespace

LuaScript::LuaScript(LuaScriptProvider& provider, const std::string& script)
  : m_provider(provider)
  , m_script(script)
{
}

bool LuaScript::execute(Arena& arena, const SignalCommand& cmd)
{
  lua_State* state = m_provider.envFor(arena);
  if (!state) {
    LOG_ERROR("Lua init error: failed to acquire state").show();
    return false;
  }

  lua_settop(state, 0);

  SignalCommand mutableCmd = cmd;
  D2L::Lua::set_cmd(state, mutableCmd);

  const auto source = loadScriptSource(m_script);
  if (!loadAndRun(state, source)) {
    return false;
  }

  bool result = false;
  if (lua_gettop(state) > 0 && lua_isboolean(state, -1)) {
    result = lua_toboolean(state, -1);
    lua_pop(state, 1);
  }

  lua_getglobal(state, "handle");
  if (lua_isfunction(state, -1)) {
    lua_getglobal(state, "cmd");
    lua_getglobal(state, "arena");
    lua_getglobal(state, "bus");
    if (lua_pcall(state, 3, 1, 0) != LUA_OK) {
      const char* msg = lua_tostring(state, -1);
      LOG_ERROR("Lua handle error: {}", msg ? msg : "unknown").show();
      lua_pop(state, 1);
    }
    else if (lua_isboolean(state, -1)) {
      result = lua_toboolean(state, -1);
      lua_pop(state, 1);
    }
    else {
      lua_pop(state, 1);
    }
  }
  else {
    lua_pop(state, 1);
  }

  return result;
}
