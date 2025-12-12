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

#include "Command.hpp"

enum class SignalCommandType : CommandType {
  CREATE = -127,  // Incoming offer, outbound call request
  ACCEPT,         // Accept session, or notification
  PROGRESS,
  RINGING,
  REJECT,
  MODIFY,
  TERMINATE
};

enum class SignalEventType : CommandType {
  CREATED = -117,  // Incoming offer, outbound call request
  ACCEPTED,        // Accept session, or notification
  PROGRESS,
  RINGING,
  REJECTED,
  MODIFIED,
  TERMINATED
};

class MediaLine;

class SIPCommand : public Command {
  using MediaLinePtr = std::unique_ptr<MediaLine>;

  CommandType m_type;
  std::list<MediaLinePtr> m_media;

 public:
  SIPCommand(SignalCommandType);
  SIPCommand(SignalEventType);

  CommandType type() const override;
  std::list<MediaLinePtr>& media();
  const std::list<MediaLinePtr>& media() const;
};
