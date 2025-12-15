/***************************************************
 * LogicLayer.hpp
 * Created on Mon, 15 Dec 2025 16:47:27 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <S2B/CommandBus.hpp>

class LogicLayer
  : public CommandBusSubscriber
  , public std::enable_shared_from_this<LogicLayer>
{
public:
  void notify(const Command& cmd) override;
  void init();
};
