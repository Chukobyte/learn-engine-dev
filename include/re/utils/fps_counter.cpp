#include "fps_counter.h"

FPSCounter::FPSCounter() : timer(new Timer((Uint32) 1000, true)) {
    timer->Start();
}

FPSCounter::~FPSCounter() {
    delete timer;
}

FPSCounter* FPSCounter::GetInstance() {
    static FPSCounter *instance = new FPSCounter();
    return instance;
}

void FPSCounter::Update() {
    if (timer->HasReachedTimeOut()) {
        timer->Stop();

        fps = fpsCount;
        fpsCount = 0;

        timer->Start();
    }
    fpsCount++;
}

unsigned int FPSCounter::GetFPS() const {
    return fps;
}
