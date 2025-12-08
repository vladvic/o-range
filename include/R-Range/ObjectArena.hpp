/***************************************************
 * ObjectArena.hpp
 * Created on Sat, 22 Nov 2025 13:20:21 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <set>
#include <memory>
#include <type_traits>
#include "Object.hpp"
#include "Session.hpp"
#include "ArenaLocator.hpp"

class Session;

class ObjectArena
  : public Object
{
  std::set<std::shared_ptr<Object>> m_objects;
public:
  ObjectArena();
  ~ObjectArena();
  ObjectArena(const ObjectArena& other) = delete;

  template<typename T>
  requires (std::is_base_of_v<Object, T>)
  void add(const std::shared_ptr<T> &o)
  {
    m_objects.emplace(std::dynamic_pointer_cast<Object>(o));
    if (o->template as<Session*>())
    {
      ArenaLocator::add(o->template as<Session*>()->id(), this->template as<std::shared_ptr<ObjectArena>>());
    }
  }

  template<typename T>
  requires (std::is_base_of_v<Object, T>)
  void remove(const std::shared_ptr<T> &o)
  {
    m_objects.erase(std::dynamic_pointer_cast<Object>(o));
    if (o->template as<Session*>())
    {
      ArenaLocator::remove(o->template as<Session*>()->id());
    }
  }
};
