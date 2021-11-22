# Initialize SDL2

## GameEngine Class

The `GameEngine` class will tie the game loop together.

Header file:
```c++
#ifndef GAME_ENGINE_H
#define GAME_ENGINE_H

#include "game_engine_context.h"
#include <game_lib/utils/logger.h>

class GameEngine {
  private:
    GameEngineContext *engineContext = nullptr;
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
```

Source file:
```c++
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
```

## Main Loop

Now that we have our `GameEngine` class defined, we can now test it to make sure everything works.

```cpp
#include "game_engine.h"

int main(int argv, char** args) {
    GameEngine gameEngine;

    while (gameEngine.IsRunning()) {
        gameEngine.ProcessInput();
        gameEngine.Update();
        gameEngine.Render();
    }

    return 0;
}
```