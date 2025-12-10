/***************************************************
 * SIPCommand.cpp
 * Created on Fri, 05 Dec 2025 12:09:55 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Command.hpp"
#include "SIPCommand.hpp"
#include "Media.hpp"

SIPCommand::SIPCommand(SIPCommandTypeEnum type)
  : m_type(type)
{
}

CommandType SIPCommand::type() const
{
  return (CommandType)m_type;
}

std::list<SIPCommand::MediaLinePtr> & SIPCommand::media()
{
  return m_media;
}

const std::list<SIPCommand::MediaLinePtr> & SIPCommand::media() const
{
  return m_media;
}

