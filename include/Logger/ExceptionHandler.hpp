/***************************************************
 * ExceptionHandler.hpp
 * Created on Mon, 15 Dec 2025 16:24:37 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/
#pragma once

#include <signal.h>
#include <unistd.h>

#include <vector>

namespace Logger
{

class ExceptionHandler
{
  std::vector<int>              m_signals;
  std::vector<struct sigaction> m_oldActions;

  static void signalHandler(int sig, siginfo_t* si, void* unused);

public:
  ExceptionHandler();
  ~ExceptionHandler();
};

} // namespace Logger
