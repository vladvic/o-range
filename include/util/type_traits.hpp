/***************************************************
 * enum.hpp
 * Created on Sat, 13 Dec 2025 22:07:49 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <iostream>
#include <limits>
#include <string_view>
#include <map>
#include <unordered_map>

namespace util
{

template<typename T>
struct type_reflection
{
  static const std::string_view name;
};

template<typename T>
auto get_type_name()
{
  std::string_view fn_name(__PRETTY_FUNCTION__);
  std::string_view name = fn_name.substr(fn_name.find("with T = ")+9);
  name = name.substr(0, name.size()-1);
  return name;
};

template<typename T>
const std::string_view type_reflection<T>::name = get_type_name<T>();

template<typename E, 
          E Min = (E)std::max((ssize_t)std::numeric_limits<std::underlying_type_t<E>>::min(), (ssize_t)-300), 
          E Max = (E)std::min((ssize_t)std::numeric_limits<std::underlying_type_t<E>>::max(), (ssize_t) 300)>
struct enum_traits
{
  class const_iterator
  {
  public:
    using iterator_category = std::input_iterator_tag;
    using value_type = const E;
    using difference_type = ssize_t;
    using pointer = const E*;
    using reference = const E;

  private:
    friend class enum_traits;

    using UnderlyingIter = std::map<E, std::string_view>::const_iterator;

    UnderlyingIter _M_val;
    explicit const_iterator(UnderlyingIter val) : _M_val(val) {}

  public:
    const_iterator() : _M_val{} {}
    const_iterator& operator++() { ++_M_val; return *this; }
    const_iterator operator++(int) { const_iterator retval = *this; ++(*this); return retval; }
    bool operator==(const_iterator other) const { return _M_val == other._M_val; }
    bool operator!=(const_iterator other) const { return !(*this == other); }
    reference operator*() const { return _M_val->first; }
  };

  static const std::string_view type_name;
  static const std::map<E, std::string_view> name;
  static const std::unordered_map<std::string_view, E> value;
  static E min() { return name.begin()->first; }
  static E max() { return name.rbegin()->first; }
  static const_iterator begin() { return const_iterator(name.begin()); }
  static const_iterator end() { return const_iterator(name.end()); }
  static const std::string_view &get_name(E e) { return name.at(e); }
  static E get_value(const std::string_view &name) { return value.at(name); }
  template<typename T>
  static bool has_value(T e) { return name.find((E)e) != name.end(); }
  template<typename F>
  static void for_each(F && f) { for (auto &s : name) { f(name.second); } }
};

template<typename E, E N>
void name_of(std::map<E, std::string_view> &_names)
{
  std::string_view fn_name(__PRETTY_FUNCTION__);
  auto enum_type_name = fn_name.substr(fn_name.find("[with E = ")+11);
  enum_type_name = enum_type_name.substr(enum_type_name.find(';'));
  auto enum_name = fn_name.substr(fn_name.find("E N = ")+6);
  if (enum_name.starts_with("("))
  {
    return;
  }
  enum_name = enum_name.substr(0, enum_name.find(']'));
  _names[N] = enum_name;
}

template<typename E, typename T, T Start, T... Ints>
auto get_enum_names_sequence(std::integer_sequence<T, Ints...>)
{
  std::map<E, std::string_view> _names;
  (name_of<E, (E)(Ints + Start)>(_names), ...);
  return _names;
}

template<typename E, E Min, E Max>
auto get_enum_names()
{
  using T = std::underlying_type_t<std::decay_t<E>>;
  constexpr ssize_t min = std::max((ssize_t)std::numeric_limits<T>::min(), (ssize_t)Min);
  constexpr ssize_t max = std::min((ssize_t)std::numeric_limits<T>::max(), (ssize_t)Max);
  return get_enum_names_sequence<E, ssize_t, min>(std::make_integer_sequence<ssize_t, max-min>{});
}

template<typename E, E Min, E Max>
auto get_enum_values()
{
  std::unordered_map<std::string_view, E> values;
  auto &_name = enum_traits<E, Min, Max>::name;
  for (auto s = _name.rbegin(); s != _name.rend(); ++s)
  {
    values.emplace(s->second, s->first);
  }
  return values;
}

template<typename E, E Min, E Max>
const std::map<E, std::string_view> enum_traits<E, Min, Max>::name = get_enum_names<E, Min, Max>();
template<typename E, E Min, E Max>
const std::unordered_map<std::string_view, E> enum_traits<E, Min, Max>::value = get_enum_values<E, Min, Max>();
template<typename E, E Min, E Max>
const std::string_view enum_traits<E, Min, Max>::type_name = type_reflection<E>::name;

} // namespace util
