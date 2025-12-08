/***************************************************
 * Session.hpp
 * Created on Sat, 22 Nov 2025 14:45:29 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <string>
#include "Object.hpp"
#include "Device.hpp"

class Session
  : public Object
{
  std::string m_id;
  std::weak_ptr<Device> m_device;

public:
  Session(const std::string& id, std::shared_ptr<Device> device = {});
  ~Session();

  inline
  std::string id() { return m_id; }
  inline
  std::shared_ptr<Device> device() { return m_device.lock(); }
};
