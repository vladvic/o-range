/***************************************************
 * Command.hpp
 * Created on Sat, 22 Nov 2025 17:09:19 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <cstddef>
#include <memory>
#include <magic_enum.hpp>

using CommandType = int8_t;

class CompletionToken
{
public:
  bool operator==(const CompletionToken &);
};

class Command
{
  std::shared_ptr<CompletionToken> m_completionToken;

public:
  virtual ~Command() = default;

  virtual CommandType type() const = 0;

  template<typename T>
  bool hasEnumType() const
  {
    return magic_enum::enum_contains<T>(type());
  }

  std::shared_ptr<CompletionToken> getCompletionToken() const;
  void setCompletionToken(std::shared_ptr<CompletionToken> t = std::make_shared<CompletionToken>());
};

