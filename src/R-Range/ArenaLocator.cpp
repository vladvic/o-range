/***************************************************
 * ArenaLocatorInterface.cpp
 * Created on Sat, 22 Nov 2025 15:06:53 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "ArenaLocator.hpp"
#include <cassert>
#include <string>

ArenaLocatorInterface* ArenaLocatorInterface::m_instance = nullptr;

ArenaLocatorInterface::ArenaLocatorInterface()
{
  assert(m_instance == nullptr);
  m_instance = this;
}

ArenaLocatorInterface::~ArenaLocatorInterface()
{
  m_instance = nullptr;
}

void ArenaLocatorInterface::add(intptr_t s, std::shared_ptr<ObjectArena> a)
{
  assert(m_instance);
  m_instance->addArena(s, a);
}

void ArenaLocatorInterface::remove(intptr_t s)
{
  assert(m_instance);
  m_instance->removeArena(s);
}

std::shared_ptr<ObjectArena> ArenaLocatorInterface::locate(intptr_t s)
{
  assert(m_instance);
  return m_instance->locateArena(s);
}

void ArenaLocatorImpl::addArena(intptr_t s, std::shared_ptr<ObjectArena>& a)
{
  m_arenas.emplace(s, a);
}

void ArenaLocatorImpl::removeArena(intptr_t s)
{
  m_arenas.erase(s);
}

std::shared_ptr<ObjectArena> ArenaLocatorImpl::locateArena(intptr_t s)
{
  if (m_arenas.find(s) != m_arenas.end()) {
    return m_arenas.at(s);
  }
  return {};
}
