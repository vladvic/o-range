/***************************************************
 * Command.cpp
 * Created on Fri, 05 Dec 2025 14:41:32 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <cstddef>
#include <memory>

#include <Command.hpp>

bool CompletionTokenTag::operator==(const CompletionTokenTag& other)
{
  return &other == this;
}

CompletionToken Command::getCompletionToken() const
{
  return m_completionToken;
}

void Command::setCompletionToken(CompletionToken t)
{
  m_completionToken = t;
}
