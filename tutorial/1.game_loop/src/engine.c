#include "engine.h"

#include <stdlib.h>
#include <SDL3/SDL.h>

#include <seika/logger.h>

// Internal engine functions

static void update_average_fps();
static void engine_update(f32 deltaTime);
static void engine_fixed_update();

// Instance of the red engine
struct REEngine {
    bool isRunning;
    REGameProperties gameProps;
    uint64 targetFPS;
    uint64 fixedUpdateInterval;
    f32 fixedDeltaTime;
};

struct REFPSTracker {
    int32 FPS;
    int32 fixedFPS;
    int32 averageFPS;
    int32 averageFixedFPS;
};

static struct REEngine engine = {0};
static struct REFPSTracker fpsTracker = {0};

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
    if (!re_is_running()) { return; } // Don't update if not running

    static uint64 currentTime = 0;
    static uint64 accumulator = 0;

    const uint64 newTime = SDL_GetTicks();
    const uint64 deltaTime = newTime - currentTime;
    currentTime = newTime;

    accumulator += deltaTime;
    while (accumulator >= engine.fixedUpdateInterval) {
        engine_fixed_update();
        fpsTracker.fixedFPS++;
        accumulator -= engine.fixedUpdateInterval;
    }

    const f32 deltaTimeSeconds = (f32)deltaTime / 1000.f;
    engine_update(deltaTimeSeconds);
    fpsTracker.FPS++;

    update_average_fps();

    if (engine.gameProps.limitFPS) {
        const uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < engine.fixedUpdateInterval) {
            SDL_Delay(engine.fixedUpdateInterval - frameTime);
        }
    }
}

void update_average_fps() {
    static uint64 lastTime = 0;
    const uint64 currentTime = SDL_GetTicks();

    if (currentTime - lastTime >= 1000) {
        ska_logger_message("FPS: %d", fpsTracker.FPS);
        ska_logger_message("FPS (fixed): %d", fpsTracker.fixedFPS);
        fpsTracker.averageFPS = fpsTracker.FPS;
        fpsTracker.averageFixedFPS = fpsTracker.fixedFPS;
        fpsTracker.FPS = 0;
        fpsTracker.fixedFPS = 0;
        lastTime = currentTime;
    }
}

void engine_update(f32 deltaTime) {}

void engine_fixed_update() {}

void re_render() {}
