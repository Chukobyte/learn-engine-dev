#include "game_engine_context.h"

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
