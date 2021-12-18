#pragma once

#include "./re/utils/timer.h"

class FPSCounter {
  public:
    ~FPSCounter();
    static FPSCounter* GetInstance();
    void Update();
    unsigned int GetFPS() const;

  private:
    unsigned int fps;
    unsigned int fpsCount;
    Timer *timer = nullptr;

    FPSCounter();
};
