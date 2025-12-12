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
#include <vector>

#include "Singleton.hpp"

namespace util
{

template<typename T>
struct is_pair
  : public std::false_type
{ };

template<typename A, typename B>
struct is_pair<std::pair<A, B>>
  : public std::true_type
{
  using FirstType  = A;
  using SecondType = B;
};

template<typename T>
constexpr bool is_pair_v = is_pair<T>::value;

class SettingsValue
{
  using KeyType   = std::string;
  using ValueType = std::variant<std::nullptr_t, 
                                 int64_t, 
                                 uint64_t, 
                                 std::string, 
                                 double, 
                                 std::vector<SettingsValue>, 
                                 std::map<KeyType, SettingsValue>>;

  ValueType m_value;

public:
  void resize(size_t s) {
    if (!(*this)) {
      m_value = std::vector<SettingsValue>{};
    }
    std::get<std::vector<SettingsValue>>(m_value).resize(s);
  }

  size_t size() {
    return std::visit(
      [](auto &t) {
        using T = std::decay_t<decltype(t)>;
        if constexpr (std::is_same_v<T, std::vector<SettingsValue>> ||
                      std::is_same_v<T, std::map<KeyType, SettingsValue>>)
        {
          return (size_t)t.size();
        }
        return (size_t)0;
      }, m_value);
  }

  template<typename Callable>
  void forEach(Callable&& callback) {
    std::visit(
      [&callback](auto &t) {
        using T = std::decay_t<decltype(t)>;
        if constexpr (std::is_same_v<T, std::vector<SettingsValue>>) {
          size_t i = 0;
          for (auto &e : t) {
            std::pair<size_t, SettingsValue&> v(i++, e);
            callback(v);
          }
        }
        else if constexpr (std::is_same_v<T, std::map<KeyType, SettingsValue>>) {
          for (auto &v : t) {
            callback(v);
          }
        }
        else {
            callback(t);
        }
      }, m_value);
  }

  std::vector<KeyType> keys() {
    std::vector<KeyType> keys;
    if (!(*this)) {
      m_value = std::map<KeyType, SettingsValue>{};
    }
    auto &map = std::get<std::map<KeyType, SettingsValue>>(m_value);
    for (auto &p : map) {
      keys.push_back(p.first);
    }
    return keys;
  }

  operator bool() const {
    return std::visit(
      [](auto &t) {
        using T = std::decay_t<decltype(t)>;
        if constexpr (std::is_same_v<T, std::nullptr_t>) {
          return false;
        }
        return true;
      }, m_value);
  }

  template<typename T>
  requires std::is_integral_v<T>
  SettingsValue &operator[](T i) {
    if (!(*this)) {
      m_value = std::vector<SettingsValue>{};
    }
    return std::get<std::vector<SettingsValue>>(m_value)[i];
  }

  template<typename T>
  requires std::is_constructible_v<KeyType, T>
  SettingsValue &operator[](const T &i) {
    if (!(*this)) {
      m_value = std::map<KeyType, SettingsValue>{};
    }
    return std::get<std::map<KeyType, SettingsValue>>(m_value)[i];
  }

  const SettingsValue &operator[](size_t i) const {
    return std::get<std::vector<SettingsValue>>(m_value)[i];
  }

  const SettingsValue &operator[](const KeyType &i) const {
    return std::get<std::map<KeyType, SettingsValue>>(m_value).at(i);
  }

  template<typename T>
  requires std::is_signed_v<T> && std::is_integral_v<T>
  T as() const {
    return std::get<int64_t>(m_value);
  }

  template<typename T>
  requires std::is_unsigned_v<T> && std::is_integral_v<T>
  T as() const {
    return std::get<uint64_t>(m_value);
  }

  template<typename T>
  requires std::is_constructible_v<T, const char*>
  T as() const {
    return T(std::get<std::string>(m_value).c_str());
  }

  template<typename T>
  requires std::is_floating_point_v<T>
  T as() const {
    return std::get<double>(m_value);
  }

  template<typename T, typename N>
  requires std::is_same_v<T, std::vector<N>>
  T as() const {
    return std::get<T>(m_value);
  }

  template<typename T>
  requires std::is_signed_v<T> && std::is_integral_v<T>
  void set(const T &v) {
    m_value = (int64_t)v;
  }

  template<typename T>
  requires std::is_unsigned_v<T> && std::is_integral_v<T>
  void set(const T &v) {
    m_value = (uint64_t)v;
  }

  template<typename T>
  requires std::is_constructible_v<std::string, const T&>
  void set(const T &v) {
    m_value = std::string(v);
  }

  template<typename T>
  requires std::is_floating_point_v<T>
  void set(const T &v) {
    m_value = (double)v;
  }

  template<typename T>
  SettingsValue &operator=(const T &v) {
    set<T>(v);
    return *this;
  }
};

class SettingsHolder
{
  using KeyType   = std::string;
  using ValueType = SettingsValue;
  std::map<KeyType, ValueType> m_values;

public:
  auto begin() { return m_values.begin(); }
  auto end() { return m_values.begin(); }
  auto begin() const { return m_values.begin(); }
  auto end() const { return m_values.begin(); }
  ValueType &operator[](const KeyType &k) { return m_values[k]; }
  const ValueType &operator[](const KeyType &k) const { return m_values.at(k); }
  ValueType &at(const KeyType &k) { return m_values.at(k); }
};

} // namespace util
