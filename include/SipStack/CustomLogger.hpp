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
        Logger::Log::log(Logger::SourceLocation(subsystem.getSubsystem().c_str(), file, line),
                        mapResipLogLevel2LogLevel(level),
                        "ReSIP: {}", message.c_str());
        return true;
    }

    static Logger::LogLevel mapResipLogLevel2LogLevel(resip::Log::Level level) {
        switch(level) {
            case resip::Log::Debug:
                return Logger::LogLevel::debug;
            case resip::Log::Info:
                return Logger::LogLevel::info;
            case resip::Log::Warning:
                return Logger::LogLevel::warn;
            case resip::Log::Err:
                return Logger::LogLevel::error;
            case resip::Log::Crit:
                return Logger::LogLevel::critical;
            default:
                return Logger::LogLevel::debug;
        }
    }

};
