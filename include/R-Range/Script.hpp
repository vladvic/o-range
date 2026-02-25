/***************************************************
 * Script.hpp
 * Created on Tue, 24 Feb 2026 20:45:54 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

class ObjectArena;
using Arena = ObjectArena;
class SignalCommand;

class Script
{
public:
  virtual ~Script() = default;
  virtual bool execute(Arena& arena, const SignalCommand& cmd) { return false; }
};
