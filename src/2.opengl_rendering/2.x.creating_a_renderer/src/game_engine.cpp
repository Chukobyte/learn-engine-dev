#include "game_engine.h"

#include <SDL2/SDL.h>

GameEngine::GameEngine() : logger(Logger::GetInstance()) {
    Initialize();
}

GameEngine::~GameEngine() {
//    SDL_Quit();
    logger->Info("Game engine shutdown!");
}

void GameEngine::Initialize() {
    logger->Info("Initializing engine...");
    InitializeSDL();
    logger->Info("Engine finished initializing!");
}

void GameEngine::InitializeSDL() {
//    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
//        logger->Error("Error on initializing SDL\n" + std::string(SDL_GetError()));
//        return;
//    }
}

void GameEngine::ProcessInput() {

}

void GameEngine::Update() {

}

void GameEngine::Render() {

}

bool GameEngine::IsRunning() const {
    return false;
}
