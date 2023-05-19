#ifndef CPPLOGGER_LOGGER_H_
#define CPPLOGGER_LOGGER_H_

#include <cstdio>
#include <ctime>
#include <mutex>

#define LOG_LEVEL_TRACE 0
#define LOG_LEVEL_DEBUG 1
#define LOG_LEVEL_INFO 2
#define LOG_LEVEL_WARN 3
#define LOG_LEVEL_ERROR 4
#define LOG_LEVEL_FATAL 5

// Macro to get function signature.
#if defined(_MSC_VER)
#  define FUNC_SIG __FUNCSIG__
#else
#  define FUNC_SIG __PRETTY_FUNCTION__
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_TRACE
#  define LOG_TRACE(message, ...)
#else
#  if defined(SRC_CODE_INFO)
#    define LOG_TRACE(message, ...)                                         \
      (Logger::Trace("[%s:%d] [%s] " message, __FILE__, __LINE__, FUNC_SIG, \
                     ##__VA_ARGS__))
#  else
#    define LOG_TRACE(message, ...) (Logger::Trace(message, ##__VA_ARGS__))
#  endif
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_DEBUG
#  define LOG_DEBUG(message, ...)
#else
#  if defined(SRC_CODE_INFO)
#    define LOG_DEBUG(message, ...)                                         \
      (Logger::Debug("[%s:%d] [%s] " message, __FILE__, __LINE__, FUNC_SIG, \
                     ##__VA_ARGS__))
#  else
#    define LOG_DEBUG(message, ...) (Logger::Debug(message, ##__VA_ARGS__))
#  endif
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_INFO
#  define LOG_INFO(message, ...)
#else
#  if defined(SRC_CODE_INFO)
#    define LOG_INFO(message, ...)                                         \
      (Logger::Info("[%s:%d] [%s] " message, __FILE__, __LINE__, FUNC_SIG, \
                    ##__VA_ARGS__))
#  else
#    define LOG_INFO(message, ...) (Logger::Info(message, ##__VA_ARGS__))
#  endif
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_WARN
#  define LOG_WARN(message, ...)
#else
#  if defined(SRC_CODE_INFO)
#    define LOG_WARN(message, ...)                                         \
      (Logger::Warn("[%s:%d] [%s] " message, __FILE__, __LINE__, FUNC_SIG, \
                    ##__VA_ARGS__))
#  else
#    define LOG_WARN(message, ...) (Logger::Warn(message, ##__VA_ARGS__))
#  endif
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_ERROR
#  define LOG_ERROR(message, ...)
#else
#  if defined(SRC_CODE_INFO)
#    define LOG_ERROR(message, ...)                                         \
      (Logger::Error("[%s:%d] [%s] " message, __FILE__, __LINE__, FUNC_SIG, \
                     ##__VA_ARGS__))
#  else
#    define LOG_ERROR(message, ...) (Logger::Error(message, ##__VA_ARGS__))
#  endif
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_FATAL
#  define LOG_FATAL(message, ...)
#else
#  if defined(SRC_CODE_INFO)
#    define LOG_FATAL(message, ...)                                         \
      (Logger::Fatal("[%s:%d] [%s] " message, __FILE__, __LINE__, FUNC_SIG, \
                     ##__VA_ARGS__))
#  else
#    define LOG_FATAL(message, ...) (Logger::Fatal(message, ##__VA_ARGS__))
#  endif
#endif

constexpr int kTimeStrSize = 128;

enum class LogLevel { TRACE, DEBUG, INFO, WARN, ERROR, FATAL };

/**
 * @brief A singleton logger class.
 */
class Logger {
private:
  std::mutex mutex_;
  char time_str_[kTimeStrSize];
  const char* time_format_ = "%Y-%m-%d %H:%M:%S";

  Logger() {}

  static inline Logger& GetInstance() {
    static Logger instance;
    return instance;
  }

  // Updates the time string to log.
  void UpdateTimeStr() {
    std::lock_guard<std::mutex> lock(mutex_);
    time_t now = time(nullptr);
    struct tm local_now;
#if defined(_WIN32)
    localtime_s(&now, &local_now);
#else
    localtime_r(&now, &local_now);
#endif
    auto num_ch = strftime(time_str_, kTimeStrSize, time_format_, &local_now);
  }

  const char* GetLogLevelStr(LogLevel log_level) {
    switch (log_level) {
      case LogLevel::TRACE:
        return "TRACE";
      case LogLevel::DEBUG:
        return "DEBUG";
      case LogLevel::INFO:
        return " INFO";
      case LogLevel::WARN:
        return " WARN";
      case LogLevel::ERROR:
        return "ERROR";
      case LogLevel::FATAL:
        return "FATAL";
    }
  }

  template <typename... Args>
  void LogImpl(LogLevel log_level, const char* message, Args... args) {
    UpdateTimeStr();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
    printf("[%s] [%s] ", time_str_, GetLogLevelStr(log_level));
    printf(message, args...);  // Custom message.
    puts("");                  // New line.
#pragma clang diagnostic pop
  }

public:
  ~Logger() {}
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  template <typename... Args>
  static inline void Trace(const char* message, Args... args) {
    GetInstance().LogImpl(LogLevel::TRACE, message, args...);
  }

  template <typename... Args>
  static inline void Debug(const char* message, Args... args) {
    GetInstance().LogImpl(LogLevel::DEBUG, message, args...);
  }

  template <typename... Args>
  static inline void Info(const char* message, Args... args) {
    GetInstance().LogImpl(LogLevel::INFO, message, args...);
  }

  template <typename... Args>
  static inline void Warn(const char* message, Args... args) {
    GetInstance().LogImpl(LogLevel::WARN, message, args...);
  }

  template <typename... Args>
  static inline void Error(const char* message, Args... args) {
    GetInstance().LogImpl(LogLevel::ERROR, message, args...);
  }

  template <typename... Args>
  static inline void Fatal(const char* message, Args... args) {
    GetInstance().LogImpl(LogLevel::FATAL, message, args...);
  }
};

#endif  // CPPLOGGER_LOGGER_H_