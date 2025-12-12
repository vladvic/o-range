/***************************************************
 * Settings.hpp
 * Created on Fri, 12 Dec 2025 06:54:56 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <cstdint>
#include <map>
#include <string>
#include <variant>

#include "Singleton.hpp"

namespace util
{

class Settings
  : public Singleton<Settings>
{
  using ValueType = std::variant<int64_t, uint64_t, std::string, double>;
  std::map<std::string, ValueType> m_values;

public:
  template<typename T>
  requires std::is_signed_v<T> && std::is_integral_v<T>
  T get(const std::string &s)
  {
    return std::get<int64_t>(m_values.at(s));
  }

  template<typename T>
  requires std::is_unsigned_v<T> && std::is_integral_v<T>
  T get(const std::string &s)
  {
    return std::get<uint64_t>(m_values.at(s));
  }

  template<typename T>
  requires std::is_constructible_v<T, const char*>
  T get(const std::string &s)
  {
    return T(std::get<std::string>(m_values.at(s)).c_str());
  }

  template<typename T>
  requires std::is_floating_point_v<T>
  T get(const std::string &s)
  {
    return std::get<double>(m_values.at(s));
  }

  template<typename T>
  requires std::is_signed_v<T> && std::is_integral_v<T>
  void set(const std::string &s, const T &v)
  {
    m_values[s] = (int64_t)v;
  }

  template<typename T>
  requires std::is_unsigned_v<T> && std::is_integral_v<T>
  void set(const std::string &s, const T &v)
  {
    m_values[s] = (uint64_t)v;
  }

  template<typename T>
  requires std::is_constructible_v<std::string, const T&>
  void set(const std::string &s, const T &v)
  {
    m_values[s] = std::string(v);
  }

  template<typename T>
  requires std::is_floating_point_v<T>
  void set(const std::string &s, const T &v)
  {
    m_values[s] = (double)v;
  }
};

} // namespace util
