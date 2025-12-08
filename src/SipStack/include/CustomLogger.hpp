#pragma once

#include <rutil/Log.hxx>
#include <Logger/Logger.hpp>

#include <format>

class CustomLogger : public resip::ExternalLogger {
public:
   bool operator()(resip::Log::Level level,
      const resip::Subsystem& subsystem, 
      const resip::Data& appName,
      const char* file,
      int line,
      const resip::Data& message,
      const resip::Data& messageWithHeaders,
      const resip::Data& instanceName) override {
        Logger::LogLevel level;
        switch(level) {
            case resip::Log::Debug:
                level = Logger::LogLevel::debug;
                break;
            case resip::Log::Info:
                level = Logger::LogLevel::info;
                break;
            case resip::Log::Warning:
                level = Logger::LogLevel::warning;
                break;
            case resip::Log::Err:
                level = Logger::LogLevel::error;
                break;
            case resip::Log::Crit:
                level = Logger::LogLevel::critical;
                break;
        }
        Logger::Log::log(Logger::SourceLocation(subsystem.getSubsystem.c_str(), file, line), level, 
                        "ReSIP: {}", message);
        return true;
      }

};
