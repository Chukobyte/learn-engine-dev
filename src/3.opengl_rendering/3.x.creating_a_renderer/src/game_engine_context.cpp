#include "game_engine_context.h"

GameEngineContext* GameEngineContext::GetInstance() {
    static GameEngineContext *instance = new GameEngineContext();
    return instance;
}

void GameEngineContext::SetRunning(bool value) {
    running = value;
}

bool GameEngineContext::IsRunning() const {
    return running;
}
