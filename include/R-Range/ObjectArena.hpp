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
#include <type_traits>
#include <typeindex>
#include <typeinfo>
#include <unordered_map>
#include <unordered_set>
#include <vector>

class Session;

class ObjectArena : public Object
{
  using ObjectWeakMap =
    std::unordered_map<std::intptr_t, std::weak_ptr<Object>>;
  using ObjectWeakVector = std::vector<std::weak_ptr<Object>>;
  using ObjectSharedVector = std::unordered_set<std::shared_ptr<Object>>;
  using TypeKey = std::type_index;
  using SessionIdType = decltype(std::declval<SessionId>().hash());
  using SessionMap = std::unordered_map<SessionIdType, std::weak_ptr<Session>>;

  ObjectSharedVector m_objects;
  std::unordered_map<TypeKey, ObjectWeakMap> m_byType;
  SessionMap m_session;

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
    m_byType[std::type_index(typeid(*o.get()))].emplace((std::intptr_t)o.get(),
                                                        o);
  }

  template<typename T>
    requires(std::is_base_of_v<Object, T>)
  void remove(const std::shared_ptr<T>& o)
  {
    auto owv = m_byType.find(std::type_index(typeid(*o.get())));
    if (owv != m_byType.end()) {
      owv->second.erase((std::intptr_t)o.get());
    }
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

  template<typename T>
    requires(std::is_base_of_v<Object, std::decay_t<T>>)
  std::vector<std::shared_ptr<std::decay_t<T>>> get()
  {
    std::vector<std::shared_ptr<std::decay_t<T>>> result;
    auto owv = m_byType.find(std::type_index(typeid(std::decay_t<T>)));
    if (owv != m_byType.end()) {
      for (auto& wp : owv->second) {
        auto sp = wp.second.lock();
        if (sp) {
          result.emplace_back(
            std::move(std::dynamic_pointer_cast<std::decay_t<T>>(sp)));
        }
      }
    }
    return result;
  }
};
