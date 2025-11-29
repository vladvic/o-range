/***************************************************
 * ArenaLocatorInterface.cpp
 * Created on Sat, 22 Nov 2025 15:06:53 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <cassert>
#include <string>
#include <string>
#include "ArenaLocator.hpp"

ArenaLocatorInterface *ArenaLocatorInterface::m_instance = nullptr;

ArenaLocatorInterface::ArenaLocatorInterface()
{
  assert(m_instance == nullptr);
  m_instance = this;
}

ArenaLocatorInterface::~ArenaLocatorInterface()
{
  m_instance = nullptr;
}

void ArenaLocatorInterface::add(const std::string& s, std::shared_ptr<ObjectArena> a)
{
  assert(m_instance);
  m_instance->addArena(s, a);
}

void ArenaLocatorInterface::remove(const std::string& s)
{
  assert(m_instance);
  m_instance->removeArena(s);
}

std::shared_ptr<ObjectArena> ArenaLocatorInterface::locate(const std::string& s)
{
  assert(m_instance);
  return m_instance->locateArena(s);
}

void ArenaLocatorImpl::addArena(const std::string& s, std::shared_ptr<ObjectArena>& a)
{
  m_arenas.emplace(s, a);
}

void ArenaLocatorImpl::removeArena(const std::string& s)
{
  m_arenas.erase(std::string(s));
}

std::shared_ptr<ObjectArena> ArenaLocatorImpl::locateArena(const std::string& s)
{
  if (m_arenas.find(std::string(s)) != m_arenas.end())
  {
    return m_arenas.at(std::string(s));
  }
  return {};
}
