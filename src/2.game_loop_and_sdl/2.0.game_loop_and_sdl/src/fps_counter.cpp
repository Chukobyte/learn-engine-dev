#include "fps_counter.h"

#include "./game_lib/utils/logger.h"

FPSCounter::FPSCounter() : timer(new Timer(1000, true)) {
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
        Logger::GetInstance()->Debug("fps = %d", fps);

        timer->Start();
    }
    fpsCount++;
}

unsigned int FPSCounter::GetFPS() const {
    return fps;
}
