#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "project_properties.h"
#include "game_engine_context.h"
#include "render_context.h"
#include "asset_manager.h"
#include "./game_lib/utils/fps_counter.h"
#include "./game_lib/utils/logger.h"

class GameEngine {
  private:
    ProjectProperties *projectProperties = nullptr;
    GameEngineContext *engineContext = nullptr;
    RenderContext *renderContext = nullptr;
    AssetManager *assetManager = nullptr;
    FPSCounter *fpsCounter = nullptr;
    Logger *logger = nullptr;

    void Initialize();

    void InitializeSDL();

    void InitializeRendering();
  public:
    GameEngine();

    ~GameEngine();

    void ProcessInput();

    void Update();

    void Render();

    bool IsRunning() const;
};


#endif //GAME_ENGINE_H
