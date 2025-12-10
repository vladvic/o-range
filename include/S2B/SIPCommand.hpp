/***************************************************
 * SipCommand.hpp
 * Created on Fri, 05 Dec 2025 12:05:56 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <list>
#include <memory>

enum class SIPCommandTypeEnum : CommandType
{
  SESSION_CREATE = -127, // Incoming offer, outbound call request
  SESSION_CREATED,       // Incoming offer, outbound call request
  SESSION_ACCEPT,        // Accept session, or notification
  SESSION_ACCEPTED,      // Accept session, or notification
  SESSION_REJECT,
  SESSION_REJECTED,
  SESSION_MODIFY,
  SESSION_MODIFIED,
  SESSION_TERMINATE,
  SESSION_TERMINATED
};

class MediaLine;

class SIPCommand
  : public Command
{
  using MediaLinePtr = std::unique_ptr<MediaLine>;

  SIPCommandTypeEnum      m_type;
  std::list<MediaLinePtr> m_media;

public:
  SIPCommand(SIPCommandTypeEnum);

  CommandType type() const override;
  std::list<MediaLinePtr> & media();
  const std::list<MediaLinePtr> & media() const;
};

