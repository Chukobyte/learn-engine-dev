#pragma once

#include <string>

enum class LogLevel : unsigned int {
    DEBUG = 0,
    WARN = 1,
    INFO = 2,
    ERROR = 3,
};

class Logger {
  private:
    Logger() = default;

  public:
    static Logger* GetInstance();
    void SetLogLevel(LogLevel level);
    bool SetLogLevelFromString(std::string logLevelString);
    LogLevel GetLogLevel() const;
    void Debug(const char *fmt, ...) const;
    void Warn(const char *fmt, ...) const;
    void Info(const char *fmt, ...) const;
    void Error(const char *fmt, ...) const;

    LogLevel logLevel = LogLevel::DEBUG;
};
