/***************************************************
 * Singleton.hpp
 * Created on Mon, 08 Dec 2025 06:34:20 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <format>
#include <stdexcept>

namespace util
{

template<typename T>
class Singleton
{
  static T * s_instance;

public:
  Singleton()
  {
    if (s_instance)
    {
      throw std::runtime_error(std::format("Singleton of type {} already exists!", typeid(T).name()));
    }

    s_instance = this;
  }
  
  virtual ~Singleton()
  {
    s_instance = nullptr;
  }
  
  static T & instance()
  {
    if (!s_instance)
    {
      throw std::runtime_error(std::format("Singleton of type {} does not exist!", typeid(T).name()));
    }

    return *s_instance;
  }
};

template<typename T>
T * Singleton<T>::s_instance = nullptr;

}
