/***************************************************
 * main.cpp
 * Created on Sat, 22 Nov 2025 13:42:51 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <memory>
#include <iostream>
#include <fstream>

#include <Logger/Logger.hpp>
#include <Logger/SPDLogger.hpp>
#include <Logger/ExceptionHandler.hpp>

using namespace Logger;

int main()
{
  ExceptionHandler ex;
  SPDLogger logger;

  LOG_DEBUG("Debug log message").show();
  LOG_INFO("Info log message").show();
  LOG_WARN("Warn log message").show();
  LOG_ERROR("Error log message").show();
  Logger::Log::flush();

  int *p = NULL;
  *p = 10;
}
