/***************************************************
 * SignalCommand.cpp
 * Created on Fri, 05 Dec 2025 12:09:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Command.hpp"
#include "SignalCommand.hpp"
#include "Media.hpp"

SignalCommand::SignalCommand(SignalCommandType type)
  : m_type((CommandType)type)
{
}

SignalCommand::SignalCommand(SignalEventType type)
  : m_type((CommandType)type)
{
}

CommandType SignalCommand::type() const
{
  return (CommandType)m_type;
}

std::list<SignalCommand::MediaLinePtr> & SignalCommand::media()
{
  return m_media;
}

const std::list<SignalCommand::MediaLinePtr> & SignalCommand::media() const
{
  return m_media;
}

