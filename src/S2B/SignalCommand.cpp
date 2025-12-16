/***************************************************
 * SignalCommand.cpp
 * Created on Fri, 05 Dec 2025 12:09:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "SignalCommand.hpp"
#include "Command.hpp"
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

std::list<SignalCommand::MediaLinePtr>& SignalCommand::media()
{
  return m_media;
}

const std::list<SignalCommand::MediaLinePtr>& SignalCommand::media() const
{
  return m_media;
}

std::string& SignalCommand::destination()
{
  return m_destination;
}

const std::string& SignalCommand::destination() const
{
  return m_destination;
}

std::string& SignalCommand::source()
{
  return m_destination;
}

const std::string& SignalCommand::source() const
{
  return m_destination;
}
