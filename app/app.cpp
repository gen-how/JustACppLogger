#include <cstdio>

#include "CppLogger/logger.h"

int main(int argc, char* argv[]) {
  // These logs will always be printed.
  Logger::Log(LogLevel::kTRACE, "This is a trace message.");
  Logger::Log(LogLevel::kDEBUG, "This is a debug message.");
  Logger::Log(LogLevel::kINFO, "This is an info message.");
  Logger::Log(LogLevel::kWARN, "This is a warning message.");
  Logger::Log(LogLevel::kERROR, "This is an error message.");
  Logger::Log(LogLevel::kFATAL, "This is a fatal message.");
  puts("============================================================");

  // These logs will be printed only if ENABLE_LOG_LEVEL is higher than the
  // corresponding log level.
  LOG_TRACE("This is a trace message with source code information.");
  LOG_DEBUG("This is a debug message with source code information.");
  LOG_INFO("This is an info message with source code information.");
  LOG_WARN("This is a warning message with source code information.");
  LOG_ERROR("This is an error message with source code information.");
  LOG_FATAL("This is a fatal message with source code information.");
  return 0;
}