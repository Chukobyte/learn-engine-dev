#pragma once

#include "utils/singleton.h"

class GameEngineContext : public Singleton<GameEngineContext> {
  public:
    GameEngineContext(singleton) {}
    const char* GetEngineVersion() const;
    const char* GetEngineName() const;
    void SetRunning(bool value);
    bool IsRunning() const;


private:
    const char *engineVersion = "0.1.0";
    const char *engineName = "Red";

    bool running = false;
};
