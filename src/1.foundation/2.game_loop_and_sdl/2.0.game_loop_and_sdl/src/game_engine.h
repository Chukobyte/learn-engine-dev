#pragma once

#include "./re/game_engine_context.h"
#include "./re/utils/logger.h"
#include "fps_counter.h"

class GameEngine {
  public:
    GameEngine();
    ~GameEngine();
    void ProcessInput();
    void Update();
    void Render();
    bool IsRunning() const;

  private:
    GameEngineContext *engineContext = nullptr;
    FPSCounter *fpsCounter = nullptr;
    Logger *logger = nullptr;

    bool Initialize();
    bool InitializeSDL();
};
