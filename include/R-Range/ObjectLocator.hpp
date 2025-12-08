/***************************************************
 * ObjectLocator.hpp
 * Created on Sat, 22 Nov 2025 15:40:06 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

template<typename T>
class ObjectLocatorInterface
{
  friend class T;
  static ObjectLocatorInterface *m_instance;

  virtual void addArena(const std::string&, std::shared_ptr<T>) = 0;
  virtual void removeArena(const std::string&) = 0;
  virtual std::shared_ptr<T> locateArena(const std::string&) = 0;

  static void add(const std::string&, std::shared_ptr<T>);
  static void remove(const std::string&);

public:
  ObjectLocatorInterface();
  virtual ~ObjectLocatorInterface();

  static std::shared_ptr<T> locate(const std::string&);
};

template<typename T>
static ObjectLocatorInterface *ObjectLocatorInterface<T>::m_instance = nullptr;
