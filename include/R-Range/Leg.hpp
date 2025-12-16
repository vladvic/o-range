/***************************************************
 * Leg.h
 * Created on Sat, 22 Nov 2025 13:17:52 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "Object.hpp"

class Session;
class Call;

class Leg : public Object
{
  std::weak_ptr<Session> m_session;
  std::weak_ptr<Call> m_call;

public:
  Leg();
  inline std::shared_ptr<Session> session() { return m_session.lock(); }
  std::shared_ptr<Call> call();
};
