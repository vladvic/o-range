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

using CommandType = std::size_t;

class Command
{
public:
  virtual ~Command() = default;

  virtual CommandType type() const = 0;

  template<class C, typename ...Args>
  static std::unique_ptr<Command> makeUnique(Args... args)
  {
    return std::unique_ptr<Command>(new C(args...));
  }
};

