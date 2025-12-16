/***************************************************
 * Call.cpp
 * Created on Sat, 22 Nov 2025 13:31:47 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Call.hpp"
#include "Leg.hpp"

std::shared_ptr<Call> Call::fromLeg(std::shared_ptr<Leg> leg)
{
  m_fromLeg = leg;
  return std::dynamic_pointer_cast<Call>(shared_from_this());
};

void Call::setInitiatingLeg(std::shared_ptr<Leg> leg)
{
  std::get<0>(m_legs) = leg;
  leg->setCall(std::dynamic_pointer_cast<Call>(shared_from_this()));
}

std::shared_ptr<Leg> Call::leg()
{
  auto leg = std::get<0>(m_legs).lock();
  if (leg == m_fromLeg) {
    leg = std::get<1>(m_legs).lock();
  }
  m_fromLeg.reset();
  return leg;
}
