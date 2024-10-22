#include "engine.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include <seika/defines.h>

#include "seika/logger.h"

#define MIN(a, b) ((a) < (b) ? (a) : (b))

typedef struct REEngine {
    bool isRunning;
    int32 FPS;
} REEgine;

static REEgine engine = { .isRunning = false, .FPS = 0 };

int re_run() {
    engine.isRunning = true;
    const uint64 TARGET_FPS = 60;
    const uint64 FIXED_UPDATE_INTERVAL = 1000 / TARGET_FPS; // 16 ms per update
    uint64 currentTime = SDL_GetTicks();
    uint64 lastTime = currentTime;
    uint64 accumulator = 0;
    int32 frames = 0;
    while (engine.isRunning) {
        const uint64 newTime = SDL_GetTicks();
        // const uint64 deltaTime = MIN(newTime - currentTime, 250);
        const uint64 deltaTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += deltaTime;
        while (accumulator >= FIXED_UPDATE_INTERVAL) {
            // fixed update
            accumulator -= FIXED_UPDATE_INTERVAL;
        }

        // update

        frames++;
        if (currentTime - lastTime >= 1000) {
            engine.FPS = frames;
            ska_logger_error("FPS: %d", engine.FPS);
            frames = 0;
            lastTime = currentTime;
        }

        // See if delay is needed
        const uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < FIXED_UPDATE_INTERVAL) {
            SDL_Delay(FIXED_UPDATE_INTERVAL - frameTime);
        }
    }

    return EXIT_SUCCESS;
}

