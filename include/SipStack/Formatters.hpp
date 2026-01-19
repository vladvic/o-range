/***************************************************
 * Formatters.hpp
 * Created on Mon, 19 Jan 2026 06:36:09 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <sstream>

#include "Logger/Formatters.hpp"

template<>
struct std::formatter<resip::SipMessage> : std::formatter<std::string>
{
  auto format(const resip::SipMessage& m, std::format_context& ctx) const
  {
    std::ostringstream s;
    s << m;
    return std::format_to(ctx.out(), "{}", s.str());
  }
};
