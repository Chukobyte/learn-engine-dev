#ifndef LOGGER_H
#define LOGGER_H

#include <string>

typedef int LogLevel;
enum _LogLevel {
    LogLevel_DEBUG = 0,
    LogLevel_WARN = 1,
    LogLevel_INFO = 2,
    LogLevel_ERROR = 3,
};

class Logger {
  private:
    Logger() = default;

  public:
    LogLevel logLevel = LogLevel_DEBUG;

    static Logger* GetInstance();

    void SetLogLevel(LogLevel level);

    bool SetLogLevelFromString(std::string logLevelString);

    LogLevel GetLogLevel() const;

    void Debug(const char *fmt, ...) const;

    void Warn(const char *fmt, ...) const;

    void Info(const char *fmt, ...) const;

    void Error(const char *fmt, ...) const;
};

#endif //LOGGER_H
