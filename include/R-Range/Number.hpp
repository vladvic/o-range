/***************************************************
 * Number.h
 * Created on Sat, 22 Nov 2025 13:17:27 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "Object.hpp"
#include <string>

class Number : public Object
{
  std::string m_number;

  std::string normalizeNumber(std::string);

public:
  Number(const std::string& s)
    : m_number(normalizeNumber(s))
  {
  }

  inline const std::string number() { return m_number; }
};
