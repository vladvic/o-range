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
#include <fstream>

#include <S2B/Command.hpp>
#include <S2B/CommandBus.hpp>
#include <Logger/Logger.hpp>
#include <util/type_traits.hpp>

using namespace Logger;

enum class PrintCommandType : CommandType
{
  PRINT_OUT_COMMAND = 100,
  PRINT_ERR_COMMAND,
};

enum class WriteCommandType : CommandType
{
  SAVE_COMMAND = 20,
};

class SimpleLogger
    : public Logger::ILoggerImpl
{
public:
  void emit_message(LogLevel lvl, SinkType sink, std::string_view msg,
      const Logger::SourceLocation &loc) override
  {
    std::cerr << msg << std::endl;
  }
};

class PrintCommand
  : public Command
{
  std::string m_what;
  CommandType m_type;

public:
  PrintCommand(const std::string &what, PrintCommandType type = PrintCommandType::PRINT_OUT_COMMAND)
    : m_what(what)
    , m_type((CommandType)type)
  { }

  const std::string &what() const { return m_what; }

  CommandType type() const
  {
    return m_type;
  }
};

class WriteCommand
  : public Command
{
  std::string m_what;

public:
  WriteCommand(const std::string &what)
    : m_what(what)
  { }

  const std::string &what() const { return m_what; }

  CommandType type() const
  {
    return (CommandType)WriteCommandType::SAVE_COMMAND;
  }
};

class SimpleSubscriber
  : public CommandBusSubscriber
{
public:
  void notify(const Command & cmd) override
  {
    if (cmd.hasEnumType<PrintCommandType>())
    {
      const auto &printCmd = dynamic_cast<const PrintCommand&>(cmd);
      std::cout << printCmd.what() << std::endl;
    }
    else if (cmd.hasEnumType<WriteCommandType>())
    {
      const auto &writeCmd = dynamic_cast<const WriteCommand&>(cmd);
      std::ofstream out("out.txt");
      out << writeCmd.what() << std::endl;
    }
  };
};

int main()
{
  SimpleLogger logger;
  auto sub = std::make_shared<SimpleSubscriber>();
  {
    MainCommandBus bus;
    CommandBus::instance().subscribe(PrintCommandType::PRINT_OUT_COMMAND, sub);
    CommandBus::instance().subscribe(PrintCommandType::PRINT_ERR_COMMAND, sub);
    CommandBus::instance().subscribe(WriteCommandType::SAVE_COMMAND, sub);

    using print_command_traits = util::enum_traits<PrintCommandType>;

    for(const auto &type : std::ranges::subrange(print_command_traits::begin(), print_command_traits::end())){
      std::cout << (int)type << ": " << print_command_traits::get_name(type) << std::endl;
    }

    for (int i = 0; i < 10; ++i)
    {
      bus.publish(std::make_unique<PrintCommand>("Hello world "+std::to_string(i)+"!"));
    }
    for (int i = 0; i < 10; ++i)
    {
      bus.publish(std::make_unique<PrintCommand>("Hello world "+std::to_string(i)+"!", PrintCommandType::PRINT_ERR_COMMAND));
    }
    for (int i = 0; i < 10; ++i)
    {
      bus.publish(std::make_unique<WriteCommand>("Hello world "+std::to_string(i)+"!"));
    }
  }
}
