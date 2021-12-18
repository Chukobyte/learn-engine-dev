# Game Loop Introduction

## Game Loop

Before we start adding other systems to ***Red Engine*** we should first create the game loop.  The game loop will be responsible for tasks such as processing input, updating game state (physics, A.I., etc...), and rendering.  Below is how our game loop will look:

```cpp
while (gameEngine.IsRunning()) {
    gameEngine.ProcessInput();
    gameEngine.Update();
    gameEngine.Render();
}
```

In the next section we will define a `GameEngine` class, but we have a few more things to cover before we do so.

## Logger

As we add more functionality and systems to the game it will be useful to make logging statements for debugging, error, etc...  The `Logger` class will be used for this purpose throughout ***Red Engine***.

```cpp
#pragma once

#include <string>

enum class LogLevel : unsigned int {
    DEBUG = 0,
    WARN = 1,
    INFO = 2,
    ERROR = 3,
};

class Logger {
  public:
    LogLevel logLevel = LogLevel::DEBUG;

    static Logger* GetInstance();
    void SetLogLevel(LogLevel level);
    bool SetLogLevelFromString(std::string logLevelString);
    LogLevel GetLogLevel() const;
    void Debug(const char *fmt, ...) const;
    void Warn(const char *fmt, ...) const;
    void Info(const char *fmt, ...) const;
    void Error(const char *fmt, ...) const;
  private:
    Logger() = default;
};
```

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
```

Right off the bat, the first thing to mention is that the `Logger` class is a [singleton](https://en.wikipedia.org/wiki/Singleton_pattern).  This means there can only be one instance of the `Logger` class can be active throughout the life of the application.  We will be using this pattern for other classes as well.  The function `GetInstance()` is used to obtain the single instance of `Logger`.

The functions `Error`, `Warn`, `Info`, and `Debug` are essentially wrappers for the `printf` function and are used to print logging statements matching the log level of the function name.  Log level can be set with either `SetLogLevel` or `SetLogLevelFromString`.  More details about the 4 log levels can be found below:

| Log Level | Description                                                      | Priority  |
|:---------:|:----------------------------------------------------------------:|:---------:|
| DEBUG     | Most verbose logging, will print debug related logs.             | 3         |
| INFO      | Will print non-important but potentially useful information.     | 2         |
| WARN      | Problems that should be fixed but aren't fatal.                  | 1         |
| ERROR     | Fatal errors that will most likely cause application errors.     | 0         |

Using log levels with a higher priority will print all logs for that level and below.  For example, a log level of `ERROR` will only print error logs and a log level of `INFO` will print info, warn, and error logs.

## Engine Context

`EngineContext` will store engine related properties and state.  There will be parts of ***Red Engine*** that need access to properties such as if the game engine is running or the engine's name.  We wouldn't want to have to pass the top level `GameEngine` class (that we haven't defined yet) to places that need it, so we're going to separate engine properties and state into it's own class `GameEngineContext`.  `GameEngineContext` will also be a singleton just like our `Logger` class.

```c++
#pragma once

class GameEngineContext {
  public:
    static GameEngineContext* GetInstance();
    const char* GetEngineVersion() const;
    const char* GetEngineName() const;
    void SetRunning(bool value);
    bool IsRunning() const;

  private:
    const char *engineVersion = "0.1.0";
    const char *engineName = "Red";
    bool running = false;

    GameEngineContext() = default;
};
```

```c++
#include "game_engine_context.h"

GameEngineContext* GameEngineContext::GetInstance() {
    static GameEngineContext *instance = new GameEngineContext();
    return instance;
}

const char* GameEngineContext::GetEngineVersion() const {
    return engineVersion;
}

const char* GameEngineContext::GetEngineName() const {
    return engineName;
}

void GameEngineContext::SetRunning(bool value) {
    running = value;
}

bool GameEngineContext::IsRunning() const {
    return running;
}
```

Nothing much to explain here.  The public functions are used to get the game engine version, name, and set/get whether the engine is running.  Now that we have a few classes defined, it's time to tackle setting up SDL!
