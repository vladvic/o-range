/***************************************************
 * ScriptKey.hpp
 * Created on Tue, 24 Feb 2026 21:15:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <S2B/SignalCommand.hpp>

class ScriptKey
{
  const SignalCommand& m_command;

public:
  explicit ScriptKey(const SignalCommand& cmd)
    : m_command(cmd)
  {
  }

  SignalEventType eventType() const
  {
    return static_cast<SignalEventType>(m_command.type());
  }

  const SignalCommand& command() const { return m_command; }
};
