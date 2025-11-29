/***************************************************
 * Session.cpp
 * Created on Sat, 22 Nov 2025 14:45:59 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Session.hpp"

Session::Session(const std::string& id, std::shared_ptr<Device> device)
  : m_id(id)
  , m_device(device)
{ }

Session::~Session()
{ }
