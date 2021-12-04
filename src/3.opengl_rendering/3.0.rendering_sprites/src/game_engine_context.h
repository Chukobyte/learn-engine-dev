#ifndef GAME_ENGINE_CONTEXT_H
#define GAME_ENGINE_CONTEXT_H

class GameEngineContext {
  public:
    static GameEngineContext* GetInstance();
    const char* GetEngineVersion() const;
    const char* GetEngineName() const;
    void SetRunning(bool value);
    bool IsRunning() const;

  private:
    const char *engineVersion = "0.1.0";
    const char *engineName = "Red";
    bool running = false;

    GameEngineContext() = default;
};

#endif //GAME_ENGINE_CONTEXT_H
