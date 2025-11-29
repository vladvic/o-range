/***************************************************
 * ObjectArena.cpp
 * Created on Sat, 22 Nov 2025 13:32:44 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "ObjectArena.hpp"
#include <Logger/Logger.hpp>

ObjectArena::ObjectArena()
{
  LOG_DEBUG("ObjectArena created!");
}

ObjectArena::~ObjectArena()
{
  LOG_DEBUG("ObjectArena destroyed!");
}
