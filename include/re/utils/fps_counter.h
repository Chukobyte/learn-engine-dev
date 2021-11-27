#ifndef FPS_COUNTER_H
#define FPS_COUNTER_H

#include "./re/utils/timer.h"

class FPSCounter {
  private:
    unsigned int fps;
    unsigned int fpsCount;
    Timer *timer = nullptr;

    FPSCounter();
  public:
    ~FPSCounter();

    static FPSCounter* GetInstance();

    void Update();

    unsigned int GetFPS() const;
};


#endif //FPS_COUNTER_H
