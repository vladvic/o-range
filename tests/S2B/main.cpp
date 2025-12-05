/***************************************************
 * main.cpp
 * Created on Sat, 22 Nov 2025 13:42:51 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <memory>
#include <iostream>

#include <S2B/Command.hpp>
#include <S2B/CommandBus.hpp>
#include <Logger/Logger.hpp>

using namespace Logger;

class SimpleLogger
    : public Logger::ILoggerImpl
{
public:
  void emit_message(LogLevel lvl, SinkType sink, std::string_view msg,
      const std::source_location loc) override
  {
    std::cerr << msg << std::endl;
  }
};

class PrintCommand
  : public Command
{
  std::string m_what;
public:
  PrintCommand(const std::string &what)
    : m_what(what)
  { }

  const std::string &what() const { return m_what; }

  static constexpr CommandType commandType = 100;
  CommandType type() const
  {
    return commandType;
  }
};

class SimpleSubscriber
  : public CommandBusSubscriber
{
public:
  void notify(const Command & cmd) override
  {
    const auto &printCmd = dynamic_cast<const PrintCommand&>(cmd);
    std::cout << printCmd.what() << std::endl;
  };
};

int main()
{
  SimpleLogger logger;
  auto sub = std::make_shared<SimpleSubscriber>();
  CommandBus bus;
  bus.subscribe(PrintCommand::commandType, sub);
  bus.publish(Command::makeUnique<PrintCommand>("Hello world!"));
}
