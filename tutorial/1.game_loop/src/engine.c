#include "engine.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include <seika/logger.h>

// Instance of the red engine
typedef struct REEngine {
    bool isRunning;
    int32 FPS;
    REGameProperties gameProps;
    uint64 targetFPS;
    uint64 fixedUpdateInterval;
} REEgine;

static REEgine engine = { .isRunning = false, .FPS = 0 };

bool re_run(REGameProperties props) {
    engine.isRunning = true;
    engine.gameProps = props;
    engine.targetFPS = props.targetFPS != NULL ? *props.targetFPS : 60;
    engine.fixedUpdateInterval = 1000 / engine.targetFPS; // 16 ms per update when targetFPS is 60
    return true;
}

bool re_is_running() {
    return engine.isRunning;
}

void re_update() {
    // Don't update if not running
    if (!re_is_running()) { return; }

    uint64 currentTime = SDL_GetTicks();
    uint64 lastTime = currentTime;
    uint64 accumulator = 0;
    int32 frames = 0;
    int32 fixedFrames = 0;
    while (engine.isRunning) {
        const uint64 newTime = SDL_GetTicks();
        // const uint64 deltaTime = MIN(newTime - currentTime, 250);
        const uint64 deltaTime = newTime - currentTime;
        currentTime = newTime;

        accumulator += deltaTime;
        while (accumulator >= engine.fixedUpdateInterval) {
            // fixed update
            fixedFrames++;
            accumulator -= engine.fixedUpdateInterval;
        }

        // update

        frames++;
        if (currentTime - lastTime >= 1000) {
            engine.FPS = frames;
            ska_logger_error("FPS: %d", engine.FPS);
            ska_logger_error("FPS (fixed): %d", fixedFrames);
            frames = 0;
            fixedFrames = 0;
            lastTime = currentTime;
        }

        if (engine.gameProps.delayCPU) {
            // See if delay is needed
            const uint64 frameTime = SDL_GetTicks() - currentTime;
            if (frameTime < engine.fixedUpdateInterval) {
                SDL_Delay(engine.fixedUpdateInterval - frameTime);
            }
        }
    }
}

void re_render() {}
