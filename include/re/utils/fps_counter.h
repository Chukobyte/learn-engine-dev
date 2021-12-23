#pragma once

#include "singleton.h"

#include "timer.h"

class FPSCounter : public Singleton<FPSCounter> {
  public:
    ~FPSCounter();
    FPSCounter(singleton);
    void Update();
    unsigned int GetFPS() const;
private:
    unsigned int fps;
    unsigned int fpsCount;

    Timer *timer = nullptr;
};
