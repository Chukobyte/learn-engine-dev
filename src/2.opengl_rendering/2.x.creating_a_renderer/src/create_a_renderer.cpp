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
