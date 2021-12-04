#include "logger.h"

#include <bits/stdc++.h>

Logger* Logger::GetInstance() {
    static Logger *instance = new Logger();
    return instance;
}

void Logger::SetLogLevel(LogLevel level) {
    logLevel = level;
}

bool Logger::SetLogLevelFromString(std::string logLevelString) {
    transform(logLevelString.begin(), logLevelString.end(), logLevelString.begin(), ::tolower);
    if (logLevelString == "debug") {
        logLevel = LogLevel::DEBUG;
        return true;
    } else if (logLevelString == "warn") {
        logLevel = LogLevel::WARN;
        return true;
    } else if (logLevelString == "info") {
        logLevel = LogLevel::INFO;
        return true;
    } else if (logLevelString == "error") {
        logLevel = LogLevel::ERROR;
        return true;
    }
    return false;
}

LogLevel Logger::GetLogLevel() const {
    return logLevel;
}

void Logger::Debug(const char *fmt, ...) const {
    if (logLevel <= LogLevel::DEBUG) {
        const std::string &logMessage = "[DEBUG] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}

void Logger::Warn(const char *fmt, ...) const {
    if (logLevel <= LogLevel::WARN) {
        const std::string &logMessage = "[WARN] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}

void Logger::Info(const char *fmt, ...) const {
    if (logLevel <= LogLevel::INFO) {
        const std::string &logMessage = "[INFO] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}

void Logger::Error(const char *fmt, ...) const {
    if (logLevel <= LogLevel::ERROR) {
        const std::string &logMessage = "[ERROR] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}
