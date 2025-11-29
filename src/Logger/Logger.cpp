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

ILoggerImpl* ILoggerImpl::s_instance = nullptr;

ILoggerImpl& ILoggerImpl::instance()
{
  if (s_instance == nullptr)
  {
    throw std::runtime_error("Logger instance does not exists!");
  }
  return *s_instance;
}

ILoggerImpl::ILoggerImpl()
{
  if (s_instance != nullptr)
  {
    throw std::runtime_error("Logger instance already exists!");
  }
  s_instance = this;
}

ILoggerImpl::~ILoggerImpl()
{
  s_instance = NULL;
}

LogEmitter::LogEmitter(ILoggerImpl& lg, LogLevel lvl, std::string msg, std::source_location loc)
  : m_logger(lg), m_level(lvl), m_msg(std::move(msg)), m_location(loc)
{
  m_logger.emit_message(m_level, SinkType::Default, m_msg, m_location);
}

LogEmitter& LogEmitter::show()
{
  m_logger.emit_message(m_level, SinkType::Console, m_msg, m_location);
  return *this;
}

} // namespace Log
