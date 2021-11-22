# Game Loop Introduction

## Game Loop

This is how the game loop will look:

```cpp
while (gameEngine.IsRunning()) {
    gameEngine.ProcessInput();
    gameEngine.Update();
    gameEngine.Render();
}
```

Before we define the `GameEngine` class there are a few more tasks that we need to complete first.

## Logger

We will create a logger class to be used everywhere throughout the engine.  `Logger` will essentially be a wrapper for the `printf` function.  There will be a total of 4 log levels:

| Log Level | Description                                                      | Priority  |
|:---------:|:----------------------------------------------------------------:|:---------:|
| DEBUG     | Most verbose logging, will print debug related logs.             | 3         |
| INFO      | Will print non-important but potentially useful information.     | 2         |
| WARN      | Problems that should be fixed but aren't fatal.                  | 1         |
| ERROR     | Fatal errors that will most likely cause application errors.     | 0         |

Header file:
```cpp
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
```

Source file:
```c++
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
        logLevel = LogLevel_DEBUG;
        return true;
    } else if (logLevelString == "warn") {
        logLevel = LogLevel_WARN;
        return true;
    } else if (logLevelString == "info") {
        logLevel = LogLevel_INFO;
        return true;
    } else if (logLevelString == "error") {
        logLevel = LogLevel_ERROR;
        return true;
    }
    return false;
}

LogLevel Logger::GetLogLevel() const {
    return logLevel;
}

void Logger::Debug(const char *fmt, ...) const {
    if (logLevel <= LogLevel_DEBUG) {
        const std::string &logMessage = "[DEBUG] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}

void Logger::Warn(const char *fmt, ...) const {
    if (logLevel <= LogLevel_WARN) {
        const std::string &logMessage = "[WARN] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}

void Logger::Info(const char *fmt, ...) const {
    if (logLevel <= LogLevel_INFO) {
        const std::string &logMessage = "[INFO] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}

void Logger::Error(const char *fmt, ...) const {
    if (logLevel <= LogLevel_ERROR) {
        const std::string &logMessage = "[ERROR] " + std::string(fmt) + "\n";
        va_list args;
        va_start(args, fmt);
        vprintf(logMessage.c_str(), args);
        va_end(args);
    }
}
```

## Engine Context

Engine context will store engine related properties and state.

Head file:
```c++
#ifndef GAME_ENGINE_CONTEXT_H
#define GAME_ENGINE_CONTEXT_H

class GameEngineContext {
  private:
    bool running = false;

    GameEngineContext() = default;

  public:
    static GameEngineContext* GetInstance();

    void SetRunning(bool value);

    bool IsRunning() const;
};

#endif //GAME_ENGINE_CONTEXT_H
```

Source file:
```c++
#include "game_engine_context.h"

GameEngineContext* GameEngineContext::GetInstance() {
    static GameEngineContext *instance = new GameEngineContext();
    return instance;
}

void GameEngineContext::SetRunning(bool value) {
    running = value;
}

bool GameEngineContext::IsRunning() const {
    return running;
}
```