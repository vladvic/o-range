/***************************************************
 * SPDLogger.hpp
 * Created on Fri, 12 Dec 2025 07:11:40 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <spdlog/spdlog.h>

#include <filesystem>
#include <functional>
#include <memory>
#include <unordered_map>
#include <vector>

#include "Logger.hpp"

#define DEFAULT_LOG_FILE "o-range.log"

namespace Logger {

class SPDLogger : public ILoggerImpl
{
  std::unordered_map<SinkType, std::shared_ptr<spdlog::logger>> m_sinks;
  std::unordered_map<SinkType, spdlog::level::level_enum> m_logLevel;
  std::filesystem::path m_fileName;

  void flush() override;
  void createDefaultLoggers();

public:
  SPDLogger();
  ~SPDLogger();

  void setLogLevel(SinkType sink, LogLevel lvl);
  void setFileName(const std::filesystem::path& name);

  void emit_message(
    LogLevel lvl,
    SinkType sink,
    std::string_view msg,
    const SourceLocation& loc = std::source_location::current()) override;
};

} // namespace Logger
