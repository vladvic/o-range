/***************************************************
 * Object.h
 * Created on Sat, 22 Nov 2025 13:20:43 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <memory>
#include <type_traits>

class Object : public std::enable_shared_from_this<Object>
{
public:
  virtual ~Object() = default;

  template<typename T, typename... Args>
    requires(std::is_base_of_v<Object, T>)
  static inline std::shared_ptr<T> make(Args&&... args)
  {
    return std::make_shared<T>(std::forward<Args>(args)...);
  }

  template<typename T>
    requires(std::is_base_of_v<Object, T>)
  static inline std::shared_ptr<T> make()
  {
    return std::make_shared<T>();
  }

  template<typename T>
    requires(std::is_base_of_v<Object, typename T::element_type> &&
             std::is_same_v<T, std::weak_ptr<typename T::element_type>>)
  T as()
  {
    return T(
      std::dynamic_pointer_cast<typename T::element_type>(shared_from_this()));
  }

  template<typename T>
    requires(std::is_base_of_v<Object, typename T::element_type> &&
             std::is_same_v<T, std::shared_ptr<typename T::element_type>>)
  T as()
  {
    return std::dynamic_pointer_cast<typename T::element_type>(
      shared_from_this());
  }

  template<typename T>
    requires(std::is_base_of_v<Object, T> && !std::is_pointer_v<T>)
  T& as()
  {
    return dynamic_cast<T&>(*this);
  }

  template<typename T>
    requires(std::is_base_of_v<Object, std::remove_pointer_t<T>> &&
             std::is_pointer_v<T>)
  T as()
  {
    return dynamic_cast<T>(this);
  }
};
