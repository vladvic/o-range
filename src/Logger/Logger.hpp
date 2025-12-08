/***************************************************
 * Logger.hpp
 * Created on Tue, 22 Nov 2025 20:34:31 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <format>
#include <memory>
#include <optional>
#include <source_location>
#include <string>
#include <string_view>
#include <unordered_map>
#include <vector>
#include <util/Singleton.hpp>

namespace Logger
{

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
  Console
};

class Log;

class ILoggerImpl
  : public util::Singleton<ILoggerImpl>
{
  friend class Log;

  public:
  virtual ~ILoggerImpl() = default;
  virtual void emit_message(LogLevel lvl, SinkType sink, std::string_view msg,
      const std::source_location loc = std::source_location::current()) = 0;

  protected:
  virtual void flush() { }
};

class LogEmitter
{
  public:
    LogEmitter(ILoggerImpl& lg, LogLevel lvl, std::string msg, std::source_location loc);
    LogEmitter& show();

  private:
    ILoggerImpl&         m_logger;
    LogLevel             m_level;
    std::string          m_msg;
    std::source_location m_location;
};

class Log
{
public:
  template<class... Args>
    static LogEmitter trace(const std::source_location loc, std::string_view f, const Args&... a)
    {
      return log(loc, LogLevel::trace, f, std::forward<const Args&>(a)...);
    }
  template<class... Args>
    static LogEmitter debug(const std::source_location loc, std::string_view f, const Args&... a)
    {
      return log(loc, LogLevel::debug, f, std::forward<const Args&>(a)...);
    }
  template<class... Args>
    static LogEmitter info(const std::source_location loc, std::string_view f, const Args&... a)
    {
      return log(loc, LogLevel::info, f, std::forward<const Args&>(a)...);
    }
  template<class... Args>
    static LogEmitter warn(const std::source_location loc, std::string_view f, const Args&... a)
    {
      return log(loc, LogLevel::warn, f, std::forward<const Args&>(a)...);
    }
  template<class... Args>
    static LogEmitter error(const std::source_location loc, std::string_view f, const Args&... a)
    {
      return log(loc, LogLevel::error, f, std::forward<const Args&>(a)...);
    }
  template<class... Args>
    inline static LogEmitter critical(const std::source_location loc, std::string_view f, const Args&... a)
    {
      return log(loc, LogLevel::critical, f, std::forward<const Args&>(a)...);
    }
  template<class... Args>
    inline static LogEmitter log(const std::source_location loc, 
        LogLevel                   level, 
        std::string_view           f, const Args&... a)
    {
      return make(loc, level, f, std::forward<const Args&>(a)...);
    }
  static void flush()
  {
    ILoggerImpl::instance().flush();
  }

private:
  template<class... Args>
    inline static LogEmitter make(const std::source_location &loc, LogLevel lvl, std::string_view f, const Args&... a)
    {
      std::string msg = std::vformat(f, std::make_format_args(std::forward<const Args&>(a)...));
      return LogEmitter{ ILoggerImpl::instance(), lvl, std::move(msg), loc };
    }
};

#define LOG_TRACE(...)    Logger::Log::trace(std::source_location::current(), __VA_ARGS__)
#define LOG_DEBUG(...)    Logger::Log::debug(std::source_location::current(), __VA_ARGS__)
#define LOG_INFO(...)     Logger::Log::info(std::source_location::current(), __VA_ARGS__)
#define LOG_WARN(...)     Logger::Log::warn(std::source_location::current(), __VA_ARGS__)
#define LOG_ERROR(...)    Logger::Log::error(std::source_location::current(), __VA_ARGS__)
#define LOG_CRITICAL(...) Logger::Log::critical(std::source_location::current(), __VA_ARGS__)
#define LOG(level, ...)   Logger::Log::log(std::source_location::current(), level, __VA_ARGS__)

} // namespace Log
