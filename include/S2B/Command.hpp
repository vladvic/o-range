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
#include <util/type_traits.hpp>

using CommandType = int16_t;

class CompletionTokenTag
{
public:
  bool operator==(const CompletionTokenTag&);
};

using CompletionToken = std::shared_ptr<CompletionTokenTag>;

class Command
{
  CompletionToken m_completionToken;

public:
  virtual ~Command() = default;

  virtual CommandType type() const = 0;

  template<typename T>
  bool hasEnumType() const
  {
    return util::enum_traits<T>::has_value(type());
  }

  CompletionToken getCompletionToken() const;
  void setCompletionToken(
    CompletionToken t = std::make_shared<CompletionTokenTag>());
};
