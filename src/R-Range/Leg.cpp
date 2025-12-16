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

Leg::Leg() {}

void Leg::setCall(std::shared_ptr<Call> call)
{
  m_call = call;
}

std::shared_ptr<Call> Leg::call()
{
  auto call = m_call.lock();
  return call->fromLeg(std::dynamic_pointer_cast<Leg>(shared_from_this()));
}
