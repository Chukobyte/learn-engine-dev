#pragma once

#include "singleton.h"

#include <string>

enum class LogLevel : unsigned int {
    DEBUG = 0,
    WARN = 1,
    INFO = 2,
    ERROR = 3,
};

class Logger : public Singleton<Logger> {
  protected:
    Logger() = default;
  public:
    Logger(singleton) {};
    void SetLogLevel(LogLevel level);
    bool SetLogLevelFromString(std::string logLevelString);
    LogLevel GetLogLevel() const;
    void Debug(const char *fmt, ...) const;
    void Warn(const char *fmt, ...) const;
    void Info(const char *fmt, ...) const;
    void Error(const char *fmt, ...) const;

    LogLevel logLevel = LogLevel::DEBUG;
};
