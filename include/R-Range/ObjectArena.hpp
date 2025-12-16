/***************************************************
 * ObjectArena.hpp
 * Created on Sat, 22 Nov 2025 13:20:21 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "ArenaLocator.hpp"
#include "Object.hpp"
#include "Session.hpp"
#include <memory>
#include <set>
#include <type_traits>

class Session;

class ObjectArena : public Object
{
  using SessionIdType = decltype(std::declval<SessionId>().hash());
  std::set<std::shared_ptr<Object>> m_objects;
  std::map<SessionIdType, std::weak_ptr<Session>> m_session;

public:
  ObjectArena();
  ~ObjectArena();
  ObjectArena(const ObjectArena& other) = delete;

  template<typename T>
    requires(std::is_base_of_v<Object, T>)
  void add(const std::shared_ptr<T>& o)
  {
    m_objects.emplace(std::dynamic_pointer_cast<Object>(o));
    if constexpr (std::is_same_v<T, Session>) {
      ArenaLocator::add(o->id().hash(),
                        this->template as<std::shared_ptr<ObjectArena>>());
      m_session.emplace(o->id().hash(), o);
    }
  }

  template<typename T>
    requires(std::is_base_of_v<Object, T>)
  void remove(const std::shared_ptr<T>& o)
  {
    m_objects.erase(std::dynamic_pointer_cast<Object>(o));
    if constexpr (std::is_same_v<T, Session>) {
      ArenaLocator::remove(o->id().hash());
      m_session.erase(o->id().hash());
    }
  }

  inline std::shared_ptr<Session> session(SessionIdType id)
  {
    return m_session.at(id).lock();
  }
};
