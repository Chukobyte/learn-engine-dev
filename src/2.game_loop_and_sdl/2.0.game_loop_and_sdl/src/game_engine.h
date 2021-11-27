#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "game_engine_context.h"
#include "fps_counter.h"
#include "./re/utils/logger.h"

class GameEngine {
  private:
    GameEngineContext *engineContext = nullptr;
    FPSCounter *fpsCounter = nullptr;
    Logger *logger = nullptr;

    void Initialize();

    void InitializeSDL();
  public:
    GameEngine();

    ~GameEngine();

    void ProcessInput();

    void Update();

    void Render();

    bool IsRunning() const;
};


#endif //GAME_ENGINE_H