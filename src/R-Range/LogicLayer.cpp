/***************************************************
 * LogicLayer.cpp
 * Created on Mon, 15 Dec 2025 16:47:15 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <util/type_traits.hpp>
#include "SignalCommand.hpp"
#include "LogicLayer.hpp"

void LogicLayer::notify(const Command& cmd) override
{
  if (cmd.hasType<SignalEventType>)
  {
    auto &signalCommand = dynamic_cast<SignalCommand&>(cmd);
  }
}

void LogicLayer::init()
{
  CommandBus::subscribe(util::enum_traits<SignalEventType>::all);
}
