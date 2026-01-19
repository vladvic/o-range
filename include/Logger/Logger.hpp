/***************************************************
 * Logger.hpp
 * Created on Tue, 22 Nov 2025 20:34:31 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#ifndef LOGGER_LOGGER_HPP
#define LOGGER_LOGGER_HPP

#include <format>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>
#include <util/Singleton.hpp>
#include <vector>

namespace Logger {

enum class LogLevel
{
  trace,
  debug,
  info,
  warn,
  error,
  critical
};

enum class SinkType
{
  Default,
  Console,
  PCAP,
  Custom1,
  Custom2
};

class Log;

class LogEmitter;
class SourceLocation
{
  const char* m_function;
  const char* m_file;
  const size_t m_line;

  friend class LogEmitter;

public:
  SourceLocation(const std::source_location&);
  SourceLocation(const char* fn, const char* file, size_t ln);

  const char* function() const noexcept { return m_function; }
  const char* file() const noexcept { return m_file; }
  const size_t line() const noexcept { return m_line; }
};

class ILoggerImpl : public util::Singleton<ILoggerImpl>
{
  friend class Log;

public:
  virtual ~ILoggerImpl() = default;
  virtual void emit_message(
    LogLevel lvl,
    SinkType sink,
    std::string_view msg,
    const SourceLocation& loc = std::source_location::current()) = 0;

protected:
  virtual void flush() {}
};

class LogEmitter
{
public:
  LogEmitter(ILoggerImpl& lg,
             LogLevel lvl,
             std::string msg,
             const SourceLocation& loc);
  LogEmitter& show(SinkType type = SinkType::Console);
  template<typename... S>
  void show(S... s)
  {
    (show(s), ...);
  }

private:
  ILoggerImpl& m_logger;
  LogLevel m_level;
  std::string m_msg;
  SourceLocation m_location;
};

class Log
{
public:
  template<class... Args>
  static LogEmitter trace(const SourceLocation& loc,
                          std::string_view f,
                          const Args&... a)
  {
    return log(loc, LogLevel::trace, f, std::forward<const Args&>(a)...);
  }
  template<class... Args>
  static LogEmitter debug(const SourceLocation& loc,
                          std::string_view f,
                          const Args&... a)
  {
    return log(loc, LogLevel::debug, f, std::forward<const Args&>(a)...);
  }
  template<class... Args>
  static LogEmitter info(const SourceLocation& loc,
                         std::string_view f,
                         const Args&... a)
  {
    return log(loc, LogLevel::info, f, std::forward<const Args&>(a)...);
  }
  template<class... Args>
  static LogEmitter warn(const SourceLocation& loc,
                         std::string_view f,
                         const Args&... a)
  {
    return log(loc, LogLevel::warn, f, std::forward<const Args&>(a)...);
  }
  template<class... Args>
  static LogEmitter error(const SourceLocation& loc,
                          std::string_view f,
                          const Args&... a)
  {
    return log(loc, LogLevel::error, f, std::forward<const Args&>(a)...);
  }
  template<class... Args>
  inline static LogEmitter critical(const SourceLocation& loc,
                                    std::string_view f,
                                    const Args&... a)
  {
    return log(loc, LogLevel::critical, f, std::forward<const Args&>(a)...);
  }
  template<class... Args>
  inline static LogEmitter log(const SourceLocation& loc,
                               LogLevel level,
                               std::string_view f,
                               const Args&... a)
  {
    return make(loc, level, f, std::forward<const Args&>(a)...);
  }
  static void flush() { ILoggerImpl::instance().flush(); }

private:
  template<class... Args>
  inline static LogEmitter make(const SourceLocation& loc,
                                LogLevel lvl,
                                std::string_view f,
                                const Args&... a)
  {
    std::string msg =
      std::vformat(f, std::make_format_args(std::forward<const Args&>(a)...));
    return LogEmitter{ ILoggerImpl::instance(), lvl, std::move(msg), loc };
  }
};

} // namespace Log

#endif

namespace Logger {

#ifdef LOG_TRACE
#undef LOG_TRACE
#endif
#ifdef LOG_DEBUG
#undef LOG_DEBUG
#endif
#ifdef LOG_INFO
#undef LOG_INFO
#endif
#ifdef LOG_WARN
#undef LOG_WARN
#endif
#ifdef LOG_ERROR
#undef LOG_ERROR
#endif
#ifdef LOG_CRITICAL
#undef LOG_CRITICAL
#endif

#define LOG_TRACE(...)                                                         \
  Logger::Log::trace(std::source_location::current(), __VA_ARGS__)
#define LOG_DEBUG(...)                                                         \
  Logger::Log::debug(std::source_location::current(), __VA_ARGS__)
#define LOG_INFO(...)                                                          \
  Logger::Log::info(std::source_location::current(), __VA_ARGS__)
#define LOG_WARN(...)                                                          \
  Logger::Log::warn(std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR(...)                                                         \
  Logger::Log::error(std::source_location::current(), __VA_ARGS__)
#define LOG_CRITICAL(...)                                                      \
  Logger::Log::critical(std::source_location::current(), __VA_ARGS__)
#define LOG(level, ...)                                                        \
  Logger::Log::log(std::source_location::current(), level, __VA_ARGS__)

} // namespace Log
