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
#include <string>

class ObjectArena;

class ArenaLocatorInterface
{
  friend class ObjectArena;
  static ArenaLocatorInterface *m_instance;

  virtual void addArena(const std::string&, std::shared_ptr<ObjectArena>&) = 0;
  virtual void removeArena(const std::string&) = 0;
  virtual std::shared_ptr<ObjectArena> locateArena(const std::string&) = 0;

  static void add(const std::string&, std::shared_ptr<ObjectArena>);
  static void remove(const std::string&);

public:
  ArenaLocatorInterface();
  virtual ~ArenaLocatorInterface();

  static std::shared_ptr<ObjectArena> locate(const std::string&);
};

class ArenaLocatorImpl
  : public ArenaLocatorInterface
{
  std::map<std::string, std::shared_ptr<ObjectArena>> m_arenas;

  void addArena(const std::string& s, std::shared_ptr<ObjectArena>& a) override;
  void removeArena(const std::string& s) override;
  std::shared_ptr<ObjectArena> locateArena(const std::string& s) override;
};

#ifndef MOCK_ARENA_LOCATOR
using ArenaLocator = ArenaLocatorImpl;
#else
using ArenaLocator = MOCK_ARENA_LOCATOR;
#endif
