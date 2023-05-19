#include "CppLogger/logger.h"

int main(int argc, char* argv[]) {
  Logger::Trace("This is a Trace message.");
  Logger::Debug("This is a Debug message.");
  Logger::Info("This is an Info message.");
  Logger::Warn("This is a Warn message.");
  Logger::Error("This is an Error message.");
  Logger::Fatal("This is a Fatal message.");
  LOG_TRACE("This is a Trace message with source code information.");
  return 0;
}