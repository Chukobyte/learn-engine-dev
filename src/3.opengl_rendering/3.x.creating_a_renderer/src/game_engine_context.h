#ifndef GAME_ENGINE_CONTEXT_H
#define GAME_ENGINE_CONTEXT_H

class GameEngineContext {
  private:
    bool running = false;

    GameEngineContext() = default;

  public:
    static GameEngineContext* GetInstance();

    void SetRunning(bool value);

    bool IsRunning() const;
};


#endif //GAME_ENGINE_CONTEXT_H
