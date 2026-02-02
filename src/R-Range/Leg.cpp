/***************************************************
 * Leg.cpp
 * Created on Sat, 22 Nov 2025 13:32:10 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Leg.hpp"
#include "Call.hpp"
#include "Session.hpp"

Leg::Leg() {}

void Leg::setCall(std::shared_ptr<Call> call)
{
  m_call = call;
}

void Leg::setSession(std::shared_ptr<Session> s)
{
  m_session = s;
  if (s->leg() != shared_from_this()) {
    s->setLeg(std::dynamic_pointer_cast<Leg>(shared_from_this()));
  }
}

std::shared_ptr<Call> Leg::call()
{
  auto call = m_call.lock();
  return call->fromLeg(std::dynamic_pointer_cast<Leg>(shared_from_this()));
}
