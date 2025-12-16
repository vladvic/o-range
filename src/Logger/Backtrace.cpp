/***************************************************
 * Backtrace.cpp
 * Created on Mon, 15 Dec 2025 16:14:49 +0000 by vladimir
 *
 * $Author$
 * $Rev$
 * $Date$
 ***************************************************/

#include <cxxabi.h>
#include <execinfo.h>

#include "Backtrace.hpp"
#include "Logger.hpp"

namespace Logger {

void printBacktrace(int levels)
{
  const int SHIFT = 3;
  int MAX_FRAMES = levels;
  void* callstack[MAX_FRAMES + SHIFT];
  int frames = backtrace(callstack, MAX_FRAMES + SHIFT);
  char** symbols = backtrace_symbols(callstack, frames);

  if (symbols == nullptr) {
    LOG_ERROR("Error: backtrace_symbols failed.");
    return;
  }

  LOG_INFO("===================== Backtrace =====================").show();
  char* demangled_name = nullptr;
  size_t demangled_length = 0;
  for (int i = SHIFT; i < frames; ++i) {
    char* symbol_str = symbols[i];
    ssize_t start_mangled = -1;
    ssize_t end_mangled = -1;

    for (int t = 0;
         symbol_str[t] && !((start_mangled >= 0) && (end_mangled >= 0));
         ++t)
    {
      if (symbol_str[t] == '(') {
        start_mangled = t;
      }
      if (symbol_str[t] == '+') {
        end_mangled = t;
      }
    }

    if (start_mangled >= 0 && end_mangled >= 0 && start_mangled < end_mangled) {
      symbol_str[end_mangled] = 0;
      symbol_str = &symbol_str[start_mangled + 1];
      int status;
      demangled_name = abi::__cxa_demangle(
        symbol_str, demangled_name, &demangled_length, &status);

      symbols[i][end_mangled] = '+';
      symbols[i][start_mangled] = 0;
      symbol_str = &symbols[i][end_mangled];

      if (status == 0) {
        LOG_INFO(
          "{}: {}({}{}", i - SHIFT, symbols[i], demangled_name, symbol_str)
          .show();
      }
      else {
        symbols[i][start_mangled] = '(';
        LOG_INFO("{}: {}", i - SHIFT, symbols[i]).show();
      }
    }
    else {
      LOG_INFO("{}: {}", i - SHIFT, symbols[i]).show();
    }
  }

  free(demangled_name);
  free(symbols);
};

} // namespace Logger
