#include "game_engine.h"

#include <SDL2/SDL.h>

GameEngine::GameEngine() :
    engineContext(GameEngineContext::GetInstance()),
    fpsCounter(FPSCounter::GetInstance()),
    logger(Logger::GetInstance()) {
    Initialize();
}

GameEngine::~GameEngine() {
    SDL_Quit();
    logger->Info("%s Engine has shut down!", engineContext->GetEngineName());
}

void GameEngine::Initialize() {
    InitializeSDL();
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);
}

void GameEngine::InitializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger->Error("Error on initializing SDL\n%s", SDL_GetError());
        return;
    }
}

void GameEngine::ProcessInput() {}

void GameEngine::Update() {
    // Sleep until FRAME_TARGET_TIME has elapsed since last frame
    const unsigned int MILLISECONDS_PER_TICK = 1000;
    const unsigned int TARGET_FPS = 60;
    static Uint32 lastFrameTime = 0;
    const unsigned int FRAME_TARGET_TIME = MILLISECONDS_PER_TICK / TARGET_FPS;
    unsigned int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    fpsCounter->Update();

    lastFrameTime = SDL_GetTicks();
}

void GameEngine::Render() {}

bool GameEngine::IsRunning() const {
    return engineContext->IsRunning();
}
