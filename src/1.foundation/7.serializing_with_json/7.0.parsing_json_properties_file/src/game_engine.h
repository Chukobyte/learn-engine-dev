#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "asset_manager.h"
#include "./re/game_engine_context.h"
#include "./re/project_properties.h"
#include "./re/rendering/renderer_2d.h"
#include "./re/rendering/render_context.h"
#include "./re/utils/fps_counter.h"
#include "./re/utils/logger.h"
#include "./re/input/input_manager.h"
#include "./re/ecs/ecs_orchestrator.h"

class GameEngine {
  public:
    GameEngine();
    ~GameEngine();
    void ProcessInput();
    void Update();
    void Render();
    bool IsRunning() const;

  private:
    ProjectProperties *projectProperties = nullptr;
    GameEngineContext *engineContext = nullptr;
    RenderContext *renderContext = nullptr;
    AssetManager *assetManager = nullptr;
    FPSCounter *fpsCounter = nullptr;
    Renderer2D *renderer2D = nullptr;
    Logger *logger = nullptr;
    InputManager *inputManager = nullptr;
    ECSOrchestrator *ecsOrchestrator = nullptr;

    bool Initialize();
    bool InitializeSDL();
    bool InitializeAudio();
    bool InitializeRendering();
    bool InitializeInput();
    bool InitializeECS();
    bool LoadProjectProperties();
};

#endif //GAME_ENGINE_H
