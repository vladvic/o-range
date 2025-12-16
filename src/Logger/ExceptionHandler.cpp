/***************************************************
 * ExceptionHandler.cpp
 * Created on Mon, 15 Dec 2025 16:25:43 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include "Backtrace.hpp"
#include "Logger.hpp"

#include "ExceptionHandler.hpp"

namespace Logger {

void ExceptionHandler::signalHandler(int sig, siginfo_t* si, void* unused)
{
  const char* exceptionCode = "";
  switch (sig) {
  case SIGSEGV:
    exceptionCode = "SIGSEGV";
    break;
  case SIGABRT:
    exceptionCode = "SIGABRT";
    break;
  case SIGFPE:
    exceptionCode = "SIGFPE";
    break;
  case SIGILL:
    exceptionCode = "SIGILL";
    break;
  case SIGBUS:
    exceptionCode = "SIGBUS";
    break;
  }
  LOG_CRITICAL("Unhandled {} at address {}",
               exceptionCode,
               static_cast<void*>(si->si_addr))
    .show();
  printBacktrace(32);
  Logger::Log::flush();
}

ExceptionHandler::ExceptionHandler()
  : m_signals({ SIGSEGV, SIGABRT, SIGFPE, SIGILL, SIGBUS })
{
  m_oldActions.resize(m_signals.size());
  for (int sigtype = 0; sigtype < m_signals.size(); ++sigtype) {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(m_signals[sigtype], &sa, &m_oldActions[sigtype]);
  }
}

ExceptionHandler::~ExceptionHandler()
{
  for (int sigtype = 0; sigtype < m_signals.size(); ++sigtype) {
    struct sigaction sa;
    sa.sa_flags = SA_SIGINFO | SA_RESETHAND;
    sa.sa_sigaction = &signalHandler;
    sigemptyset(&sa.sa_mask);
    sigaction(m_signals[sigtype], &m_oldActions[sigtype], nullptr);
  }
}

} // namespace Logger
