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
    void LogEntry(const std::string &logLevelPrefix, const std::string &logMessage) const;

  public:
    LogLevel logLevel = LogLevel_INFO;

    static Logger* GetInstance();

    void SetLogLevel(LogLevel level);

    bool SetLogLevelFromString(std::string logLevelString);

    LogLevel GetLogLevel() const;
    void Debug(const std::string &logMessage) const;
    void Warn(const std::string &logMessage) const;
    void Info(const std::string &logMessage) const;
    void Error(const std::string &logMessage) const;

    static void Assert(bool passingCondition, const std::string &logMessage, const int exitCodeOnError = 1);

    void LogOpenGLError(const std::string &context) const;
};

#endif //LOGGER_H
