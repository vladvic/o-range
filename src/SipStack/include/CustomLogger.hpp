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
        std::string msgStr = std::format("[{}:{} {}]: {}", file, line, subsystem.getSubsystem().c_str(), message.c_str());
        switch(level) {
            case resip::Log::Debug:
                LOG_DEBUG(msgStr);
                break;
            case resip::Log::Info:
                LOG_INFO(msgStr);
                break;
            case resip::Log::Warning:
                LOG_WARN(msgStr);
                break;
            case resip::Log::Err:
                LOG_ERROR(msgStr);
                break;
            case resip::Log::Crit:
                LOG_CRITICAL(msgStr);
                break;
        }
        return true;
      }

};