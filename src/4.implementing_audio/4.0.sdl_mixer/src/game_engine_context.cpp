#include "game_engine_context.h"

GameEngineContext* GameEngineContext::GetInstance() {
    static GameEngineContext *instance = new GameEngineContext();
    return instance;
}

const char* GameEngineContext::GetEngineVersion() const {
    return engineVersion;
}

const char* GameEngineContext::GetEngineName() const {
    return engineName;
}

void GameEngineContext::SetRunning(bool value) {
    running = value;
}

bool GameEngineContext::IsRunning() const {
    return running;
}
