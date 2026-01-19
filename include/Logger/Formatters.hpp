/***************************************************
 * Formatters.hpp
 * Created on Mon, 19 Jan 2026 06:37:22 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "util/type_traits.hpp"

template<typename T>
  requires std::is_enum_v<T>
struct std::formatter<T> : std::formatter<std::string>
{
  auto format(T o, std::format_context& ctx) const
  {
    return std::format_to(ctx.out(), "{}", util::enum_traits<T>::get_name(o));
  }
};
