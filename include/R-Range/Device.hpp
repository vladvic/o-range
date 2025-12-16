/***************************************************
 * Device.hpp
 * Created on Sat, 22 Nov 2025 13:17:41 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "Number.hpp"
#include "Object.hpp"
#include <string>

class Device : public Object
{
  std::string m_id;
  std::weak_ptr<Number> m_number;

public:
  Device(const std::string& id, std::shared_ptr<Number> number = {});
  ~Device();

  inline std::shared_ptr<Number> number() { return m_number.lock(); }
};
