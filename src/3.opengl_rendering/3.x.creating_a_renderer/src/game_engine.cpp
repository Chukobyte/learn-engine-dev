#include "game_engine.h"

#include <SDL2/SDL.h>

GameEngine::GameEngine() : engineContext(GameEngineContext::GetInstance()), logger(Logger::GetInstance()) {
    Initialize();
}

GameEngine::~GameEngine() {
    SDL_Quit();
    logger->Info("Game engine has shutdown!");
}

void GameEngine::Initialize() {
    InitializeSDL();
    logger->Info("Game engine has started!");
}

void GameEngine::InitializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger->Error("Error on initializing SDL\n%s", SDL_GetError());
        return;
    }
}

void GameEngine::ProcessInput() {}

void GameEngine::Update() {}

void GameEngine::Render() {}

bool GameEngine::IsRunning() const {
    return engineContext->IsRunning();
}
