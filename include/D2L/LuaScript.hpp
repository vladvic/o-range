/***************************************************
 * LuaScript.hpp
 * Created on Tue, 24 Feb 2026 20:48:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <R-Range/Script.hpp>
#include <string>

class LuaScriptProvider;

class LuaScript : public Script
{
public:
  LuaScript(LuaScriptProvider& provider, const std::string& script);
  virtual ~LuaScript() = default;
  bool execute(Arena& arena, const SignalCommand& cmd) override;

private:
  LuaScriptProvider& m_provider;
  std::string m_script;
};
