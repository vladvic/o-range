/***************************************************
 * Device.cpp
 * Created on Sat, 22 Nov 2025 13:32:26 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Device.hpp"
#include <Logger/Logger.hpp>
#include <iostream>

Device::Device(const std::string& id, std::shared_ptr<Number> number)
  : m_id(id)
  , m_number(number)
{
  LOG_DEBUG("Device {} created!", m_id);
}

Device::~Device()
{
  LOG_DEBUG("Device {} deleted!", m_id);
}
