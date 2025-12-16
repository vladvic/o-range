/***************************************************
 * Call.hpp
 * Created on Sat, 22 Nov 2025 13:18:10 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include "Object.hpp"
#include <tuple>

class Leg;

class Call : public Object
{
  friend class Leg;

  std::tuple<std::weak_ptr<Leg>, std::weak_ptr<Leg>> m_legs;
  std::shared_ptr<Leg> m_fromLeg;

  std::shared_ptr<Call> fromLeg(std::shared_ptr<Leg> leg);

public:
  std::shared_ptr<Leg> leg();
};
