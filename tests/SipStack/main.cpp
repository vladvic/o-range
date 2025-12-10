#include <SipStack.hpp>
#include <SIPCommand.hpp>
#include <Media.hpp>
#include <Logger/Logger.hpp>
#include <thread>

class SimpleLogger : public Logger::ILoggerImpl {
 public:
  void emit_message(Logger::LogLevel lvl, Logger::SinkType sink,
                    std::string_view msg,
                    const Logger::SourceLocation& loc) override {
    std::cerr << msg << std::endl;
  }
};

int main() {
  try {
    SimpleLogger logger;
    MainCommandBus bus;
    auto stack = std::make_shared<SipStack>();
    stack->subscribe(bus); // Can be replaced with CommandBus::instance().subscribe(...);

    std::this_thread::sleep_for(std::chrono::seconds(5));
    bus.publish(std::make_unique<SIPCommand>(SIPCommandTypeEnum::SESSION_CREATE));
    std::cout << "Publish SIP command" << std::endl;

    // Keep the application running to process SIP messages
    while (true) {
      std::this_thread::sleep_for(std::chrono::seconds(1));
    }
  } catch (const std::exception& e) {
    std::cerr << "Exception: " << e.what() << std::endl;
    return EXIT_FAILURE;
  }

  return EXIT_SUCCESS;
}
