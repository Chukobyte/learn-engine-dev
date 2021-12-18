#pragma once

#include <SDL2/SDL.h>

class Timer {
  public:
    Timer(Uint32 waitTimeInMilliseconds, bool doesLoop = false);
    Timer(float waitTimeInSeconds, bool doesLoop = false);
    Uint32 GetWaitTime() const;
    Uint32 GetTimeLeft() const;
    void SetWaitTime(Uint32 waitTimeInMilliseconds);
    void SetWaitTime(float waitTimeInSeconds);
    bool DoesLoop() const;
    void SetLoop(bool doesLoop);
    void Start();
    void Stop();
    void Pause();
    void UnPause();
    bool HasStopped() const;
    bool HasReachedTimeOut() const;
    Uint32 GetTicks() const;
    bool HasStarted() const;
    bool IsPaused() const;

  private:
    Uint32 waitTime;
    bool loops;
    Uint32 startTicks = 0;
    Uint32 pausedTicks = 0;
    bool isPaused = false;
    bool hasStarted = false;
};
