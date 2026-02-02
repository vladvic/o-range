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

enum class SignalCommandRejectReason
{
  BAD_REQUEST,
  BUSY,
  TIMEOUT,
  TERMINATED,
  FORBIDDEN,
  NOT_FOUND,
  NOT_ACCEPTABLE_HERE,
  INTERNAL_ERROR
};

enum class SignalCommandType : CommandType
{
  CREATE = -127, // Incoming offer, outbound call request
  ACCEPT,        // Accept session, or notification
  PROGRESS,
  RINGING,
  REJECT,
  MODIFY,
  TERMINATE,
  INVALID
};

enum class SignalEventType : CommandType
{
  CREATED = -110, // Incoming offer, outbound call request
  ACCEPTED,       // Accept session, or notification
  PROGRESS,
  RINGING,
  REJECTED,
  MODIFIED,
  TERMINATED,
  INVALID
};

class MediaLine;

class SignalCommand : public Command
{
  using MediaLinePtr = std::shared_ptr<MediaLine>;

  CommandType m_type;
  std::list<MediaLinePtr> m_media;
  std::string m_source;
  std::string m_destination;
  SignalCommandRejectReason m_rejectReason;

public:
  SignalCommand(SignalCommandType);
  SignalCommand(SignalEventType);

  CommandType type() const override;
  std::list<MediaLinePtr>& media();
  const std::list<MediaLinePtr>& media() const;
  std::string& source();
  const std::string& source() const;
  std::string& destination();
  const std::string& destination() const;
  SignalCommandRejectReason& rejectReason();
  const SignalCommandRejectReason& rejectReason() const;
};
