/***************************************************
 * Command.hpp
 * Created on Sat, 22 Nov 2025 17:09:19 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

enum class CommandType
{
  SESSION_CREATE = 0, // Incoming offer, outbound call request
  SESSION_ACCEPT,     // Accept session, or notification
  SESSION_REJECT,
  SESSION_MODIFY,
  SESSION_TERMINATE
};

class Command
{
public:
  virtual ~Command() = default;

  virtual CommandType type();
};

