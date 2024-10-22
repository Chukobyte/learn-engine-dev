#include "engine.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include <seika/logger.h>

static void internal_track_stats();
static void internal_update(f32 deltaTime);
static void internal_fixed_update();

// Instance of the red engine
typedef struct REEngine {
    bool isRunning;
    int32 FPS;
    int32 fixedFPS;
    REGameProperties gameProps;
    uint64 targetFPS;
    uint64 fixedUpdateInterval;
    f32 fixedDeltaTime;
} REEgine;

static REEgine engine = { .isRunning = false, .FPS = 0, .fixedFPS = 0 };

bool re_run(REGameProperties props) {
    engine.isRunning = true;
    engine.gameProps = props;
    engine.targetFPS = props.targetFPS != NULL ? *props.targetFPS : 60;
    engine.fixedUpdateInterval = 1000 / engine.targetFPS; // 16 ms per update when targetFPS is 60
    engine.fixedDeltaTime = (f32)engine.fixedUpdateInterval / 1000.0f;
    return true;
}

bool re_is_running() {
    return engine.isRunning;
}

void re_update() {
    // Don't update if not running
    if (!re_is_running()) { return; }

    static uint64 currentTime = 0;
    static uint64 accumulator = 0;

    const uint64 newTime = SDL_GetTicks();
    const uint64 deltaTime = newTime - currentTime;
    currentTime = newTime;

    accumulator += deltaTime;
    while (accumulator >= engine.fixedUpdateInterval) {
        internal_fixed_update();
        engine.fixedFPS++;
        accumulator -= engine.fixedUpdateInterval;
    }

    const f32 deltaTimeSeconds = (f32)deltaTime / 1000.f;
    internal_update(deltaTimeSeconds);
    engine.FPS++;

    internal_track_stats();

    if (engine.gameProps.delayCPU) {
        // See if delay is needed
        const uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < engine.fixedUpdateInterval) {
            SDL_Delay(engine.fixedUpdateInterval - frameTime);
        }
    }
}

void internal_track_stats() {
    static uint64 lastTime = 0;
    const uint64 currentTime = SDL_GetTicks();

    if (currentTime - lastTime >= 1000) {
        ska_logger_error("FPS: %d", engine.FPS);
        ska_logger_error("FPS (fixed): %d", engine.fixedFPS);
        engine.FPS = 0;
        engine.fixedFPS = 0;
        lastTime = currentTime;
    }
}

void internal_update(f32 deltaTime) {}

void internal_fixed_update() {}

void re_render() {}
