/***************************************************
 * Session.hpp
 * Created on Sat, 22 Nov 2025 14:45:29 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "Device.hpp"
#include "Object.hpp"
#include <string>

class Leg;

class SessionId
{
public:
  virtual ~SessionId() = default;
  virtual intptr_t hash() const = 0;
};

class Session : public Object
{
  std::unique_ptr<SessionId> m_id;
  std::weak_ptr<Device> m_device;
  std::weak_ptr<Leg> m_leg;

public:
  Session(std::unique_ptr<SessionId>&& id, std::shared_ptr<Device> device = {});
  ~Session();

  inline const SessionId& id() const { return *m_id; }
  inline std::shared_ptr<Device> device() { return m_device.lock(); }
  inline std::shared_ptr<Leg> leg() { return m_leg.lock(); }
};
