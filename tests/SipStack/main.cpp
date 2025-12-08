#include <SipStack.hpp>
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
    SipStack stack;

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