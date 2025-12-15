/***************************************************
 * Logger.cpp
 * Created on Tue, 22 Nov 2025 20:41:59 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#include "Logger.hpp"

#include <stdexcept>

namespace Logger
{

SourceLocation::SourceLocation(const std::source_location &sl)
  : m_function(sl.function_name())
  , m_file(sl.file_name())
  , m_line(sl.line())
{
}

SourceLocation::SourceLocation(const char * fn, const char *file, size_t ln)
  : m_function(fn)
  , m_file(file)
  , m_line(ln)
{
}

LogEmitter::LogEmitter(ILoggerImpl& lg, LogLevel lvl, std::string msg, const SourceLocation &loc)
  : m_logger(lg), m_level(lvl), m_msg(std::move(msg)), m_location(loc)
{
  m_logger.emit_message(m_level, SinkType::Default, m_msg, m_location);
}

LogEmitter& LogEmitter::show(SinkType type)
{
  m_logger.emit_message(m_level, type, m_msg, m_location);
  return *this;
}

} // namespace Log
