/***************************************************
 * SPDLogger.cpp
 * Created on Fri, 12 Dec 2025 06:52:32 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <spdlog/async.h>
#include <spdlog/details/registry.h>
#include <spdlog/details/thread_pool.h>
#include <spdlog/sinks/basic_file_sink.h>
#include <spdlog/sinks/callback_sink.h>
#include <spdlog/sinks/stdout_color_sinks.h>

#include "SPDLogger.hpp"

namespace Logger {

static constexpr inline spdlog::level::level_enum to_spd(LogLevel l)
{
  using L = LogLevel;
  switch (l) {
  case L::trace:
    return spdlog::level::trace;
  case L::debug:
    return spdlog::level::debug;
  case L::info:
    return spdlog::level::info;
  case L::warn:
    return spdlog::level::warn;
  case L::error:
    return spdlog::level::err;
  case L::critical:
    return spdlog::level::critical;
  }
  return spdlog::level::info;
}

static constexpr inline LogLevel from_spd(spdlog::level::level_enum l)
{
  using L = LogLevel;
  switch (l) {
  case spdlog::level::trace:
    return L::trace;
  case spdlog::level::debug:
    return L::debug;
  case spdlog::level::info:
    return L::info;
  case spdlog::level::warn:
    return L::warn;
  case spdlog::level::err:
    return L::error;
  case spdlog::level::critical:
    return L::critical;
  }
  return L::info;
}

SPDLogger::SPDLogger(const std::filesystem::path& logPath)
  : m_logLevel({
      { SinkType::Default, spdlog::level::info },
      { SinkType::Console, spdlog::level::info },
      { SinkType::PCAP, spdlog::level::trace },
    })
  , m_fileName(logPath)
{
  createDefaultLoggers();
}

SPDLogger::~SPDLogger()
{
  flush();
  spdlog::shutdown();
}

void SPDLogger::createDefaultLoggers()
{
  auto& registry_inst = spdlog::details::registry::instance();

  // create global thread pool if not already exists..
  auto& mutex = registry_inst.tp_mutex();
  std::lock_guard<std::recursive_mutex> tp_lock(mutex);
  auto tp = registry_inst.get_tp();
  if (tp == nullptr) {
    tp = std::make_shared<spdlog::details::thread_pool>(
      spdlog::details::default_async_q_size, 1U);
    registry_inst.set_tp(tp);
  }

#if defined(SPDLOG_WCHAR_FILENAMES)
  auto file_sink =
    std::make_shared<spdlog::sinks::basic_file_sink_mt>(m_fileName.wstring());
#else
  auto file_sink =
    std::make_shared<spdlog::sinks::basic_file_sink_mt>(m_fileName.string());
#endif
  m_sinks[SinkType::Default] = std::make_shared<spdlog::async_logger>(
    "default", file_sink, spdlog::thread_pool());
  m_sinks[SinkType::Default]->set_level(m_logLevel[SinkType::Default]);

  auto stdout_sink = std::make_shared<spdlog::sinks::stdout_color_sink_mt>();
  m_sinks[SinkType::Console] = std::make_shared<spdlog::async_logger>(
    "console", stdout_sink, spdlog::thread_pool());
  m_sinks[SinkType::Console]->set_level(m_logLevel[SinkType::Console]);

  m_sinks[SinkType::Default]->set_pattern(
    "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");
  m_sinks[SinkType::Console]->set_pattern(
    "[%Y-%m-%d %H:%M:%S.%e] [%^%l%$] [%s:%#] %v");

  spdlog::set_default_logger(m_sinks[SinkType::Console]);
}

void SPDLogger::setFileName(const std::filesystem::path& name)
{
  auto it = m_sinks.find(SinkType::Default);
  if (it == m_sinks.end() || !it->second) {
    return;
  }

#if defined(SPDLOG_WCHAR_FILENAMES)
  auto file_sink =
    std::make_shared<spdlog::sinks::basic_file_sink_mt>(m_fileName.wstring());
#else
  auto file_sink =
    std::make_shared<spdlog::sinks::basic_file_sink_mt>(m_fileName.string());
#endif

  auto sink = it->second;
  auto& registry_inst = spdlog::details::registry::instance();
  auto& mutex = registry_inst.tp_mutex();

  std::lock_guard<std::recursive_mutex> tp_lock(mutex);
  sink->sinks().clear();
  sink->sinks().push_back(std::move(file_sink));
}

void SPDLogger::setLogLevel(SinkType _sink, LogLevel lvl)
{
  auto level = to_spd(lvl);
  m_logLevel[_sink] = level;

  auto it = m_sinks.find(_sink);
  if (it == m_sinks.end() || !it->second) {
    return;
  }

  auto sink = it->second;
  auto& registry_inst = spdlog::details::registry::instance();
  auto& mutex = registry_inst.tp_mutex();

  std::lock_guard<std::recursive_mutex> tp_lock(mutex);
  sink->set_level(level);
}

void SPDLogger::emit_message(LogLevel lvl,
                             SinkType _sink,
                             std::string_view msg,
                             const SourceLocation& loc)
{
  auto it = m_sinks.find(_sink);
  if (it == m_sinks.end() || !it->second) {
    return;
  }

  auto sink = it->second;
  auto spd_lvl = to_spd(lvl);
  sink->log({ loc.file(), (int)loc.line(), loc.function() }, spd_lvl, msg);
}

void SPDLogger::flush()
{
  for (auto& sink : m_sinks) {
    if (sink.second) {
      sink.second->flush();
    }
  }
  auto& registry_inst = spdlog::details::registry::instance();
  auto& mutex = registry_inst.tp_mutex();
  std::lock_guard<std::recursive_mutex> tp_lock(mutex);
  auto tp = registry_inst.get_tp();
  if (tp) {
    while (tp->queue_size())
      ;
  }
}

} // namespace Logger
