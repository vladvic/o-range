/***************************************************
 * main.cpp
 * Created on Mon, 15 Dec 2025 16:45:12 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <S2B/SignalCommand.hpp>
#include <S2B/Media.hpp>
#include <SipStack/SipStack.hpp>
#include <R-Range/LogicLayer.hpp>
#include <Logger/SPDLogger.hpp>
#include <Logger/ExceptionHandler.hpp>
#include <thread>

int main(int argc, char **argv) {
  ExceptionHandler exceptionHandler;
  SPDLogger        logger;
  MainCommandBus   bus;

  auto stack = std::make_shared<SipStack>();
  auto logic = std::make_shared<LogicLayer>();
  logic->init();
}
