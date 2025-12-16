/***************************************************
 * Session.cpp
 * Created on Sat, 22 Nov 2025 14:45:59 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Session.hpp"

Session::Session(std::unique_ptr<SessionId>&& id,
                 std::shared_ptr<Device> device)
  : m_id(std::move(id))
  , m_device(device)
{
}

Session::~Session() {}
