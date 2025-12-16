/***************************************************
 * ArenaLocatorInterface.hpp
 * Created on Sat, 22 Nov 2025 15:01:06 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <map>
#include <memory>
#include <string>

class ObjectArena;

class ArenaLocatorInterface
{
  friend class ObjectArena;
  static ArenaLocatorInterface* m_instance;

  virtual void addArena(intptr_t, std::shared_ptr<ObjectArena>&) = 0;
  virtual void removeArena(intptr_t) = 0;
  virtual std::shared_ptr<ObjectArena> locateArena(intptr_t) = 0;

  static void add(intptr_t, std::shared_ptr<ObjectArena>);
  static void remove(intptr_t);

public:
  ArenaLocatorInterface();
  virtual ~ArenaLocatorInterface();

  static std::shared_ptr<ObjectArena> locate(intptr_t);
};

class ArenaLocatorImpl : public ArenaLocatorInterface
{
  std::map<intptr_t, std::shared_ptr<ObjectArena>> m_arenas;

  void addArena(intptr_t s, std::shared_ptr<ObjectArena>& a) override;
  void removeArena(intptr_t s) override;
  std::shared_ptr<ObjectArena> locateArena(intptr_t s) override;
};

#ifndef MOCK_ARENA_LOCATOR
using ArenaLocator = ArenaLocatorImpl;
#else
using ArenaLocator = MOCK_ARENA_LOCATOR;
#endif
