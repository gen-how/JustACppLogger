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

#if defined(SRC_CODE_INFO)
#  define LOG_MSG(log_level, message, ...)                         \
    (Logger::Log(log_level, __LINE__, __FILE__, FUNC_SIG, message, \
                 ##__VA_ARGS__))
#else
#  define LOG_MSG(log_level, message, ...) \
    (Logger::Log(log_level, message, ##__VA_ARGS__))
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_TRACE
#  define LOG_TRACE(message, ...)
#else
#  define LOG_TRACE(message, ...) \
    (LOG_MSG(LogLevel::kTRACE, message, ##__VA_ARGS__))
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_DEBUG
#  define LOG_DEBUG(message, ...)
#else
#  define LOG_DEBUG(message, ...) \
    (LOG_MSG(LogLevel::kDEBUG, message, ##__VA_ARGS__))
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_INFO
#  define LOG_INFO(message, ...)
#else
#  define LOG_INFO(message, ...) \
    (LOG_MSG(LogLevel::kINFO, message, ##__VA_ARGS__))
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_WARN
#  define LOG_WARN(message, ...)
#else
#  define LOG_WARN(message, ...) \
    (LOG_MSG(LogLevel::kWARN, message, ##__VA_ARGS__))
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_ERROR
#  define LOG_ERROR(message, ...)
#else
#  define LOG_ERROR(message, ...) \
    (LOG_MSG(LogLevel::kERROR, message, ##__VA_ARGS__))
#endif

#if ENABLE_LOG_LEVEL > LOG_LEVEL_FATAL
#  define LOG_FATAL(message, ...)
#else
#  define LOG_FATAL(message, ...) \
    (LOG_MSG(LogLevel::kFATAL, message, ##__VA_ARGS__))
#endif

constexpr int kTimeStrSize = 128;

enum class LogLevel { kTRACE, kDEBUG, kINFO, kWARN, kERROR, kFATAL };

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
      case LogLevel::kTRACE:
        return "TRACE";
      case LogLevel::kDEBUG:
        return "DEBUG";
      case LogLevel::kINFO:
        return " INFO";
      case LogLevel::kWARN:
        return " WARN";
      case LogLevel::kERROR:
        return "ERROR";
      case LogLevel::kFATAL:
        return "FATAL";
      default:
        return "     ";  // To avoid warning given by gcc.
    }
  }

  /**
   * @brief Logs a message with the given custom message.
   *
   * @param log_level Log level. (e.g. LogLevel::kINFO)
   * @param message Custom message to log.
   * @param args Arguments for the custom message.
   */
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

  /**
   * @brief Logs a message with the given line number, filepath, function
   *        signature, and custom message.
   *
   * @param log_level Log level. (e.g. LogLevel::kINFO)
   * @param line Line number.
   * @param filepath Source code filepath.
   * @param func_sig Function signature.
   * @param message Custom message to log.
   * @param args Arguments for the custom message.
   */
  template <typename... Args>
  void LogImpl(LogLevel log_level, int line, const char* filepath,
               const char* func_sig, const char* message, Args... args) {
    UpdateTimeStr();
#pragma clang diagnostic push
#pragma clang diagnostic ignored "-Wformat-security"
    printf("[%s] [%s] [%s:%d] [%s] ", time_str_, GetLogLevelStr(log_level),
           filepath, line, func_sig);
    printf(message, args...);  // Custom message.
    puts("");                  // New line.
#pragma clang diagnostic pop
  }

public:
  ~Logger() {}
  Logger(const Logger&) = delete;
  Logger& operator=(const Logger&) = delete;

  template <typename... Args>
  static inline void Log(LogLevel log_level, const char* message,
                         Args... args) {
    GetInstance().LogImpl(log_level, message, args...);
  }

  template <typename... Args>
  static inline void Log(LogLevel log_level, int line, const char* filepath,
                         const char* func_sig, const char* message,
                         Args... args) {
    GetInstance().LogImpl(log_level, line, filepath, func_sig, message,
                          args...);
  }
};

#endif  // CPPLOGGER_LOGGER_H_