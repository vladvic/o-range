/***************************************************
 * Number.cpp
 * Created on Sat, 22 Nov 2025 13:32:37 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Number.hpp"

std::string Number::normalizeNumber(std::string s)
{
  size_t pos;

  while ((pos = s.find_first_of("- .")) != std::string::npos) {
    s.replace(pos, 1, "");
  }
  return s;
}
