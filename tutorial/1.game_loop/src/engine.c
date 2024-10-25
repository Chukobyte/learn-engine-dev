#include "engine.h"

#include <stdlib.h>

#include <SDL3/SDL.h>

#include <seika/logger.h>

// Represents an instance of the red engine
struct REEngine {
    // Whether the engine is running or not
    bool isRunning;
    // The properties that are used to run the current game
    REGameProperties gameProps;
    // Target FPS for the engine's game loop
    uint64 targetFPS;
    // Actual update interval for the game loop
    uint64 updateInterval;
    // The update interval used when doing an internal fixed update
    uint64 fixedUpdateInterval;
    // Fixed delta time that can be used for fixed update calculations
    f32 fixedDeltaTime;
};

// FPS related stats
struct REFPSTracker {
    // FPS that's currently being tracked, zeroed out after 'x' seconds
    int32 FPS;
    // Same as FPS but in the context of the fixed update loop
    int32 fixedFPS;
};

// Will update the average fps
static void update_average_fps();
// Internal engine variable delta update
static void engine_update(f32 deltaTime);
// Internal engine fixed update
static void engine_fixed_update();

static struct REEngine engine = {0};
static struct REFPSTracker fpsTracker = {0};
static REGameStats gameStats = {0};

bool re_run(REGameProperties props) {
    engine.isRunning = true;
    engine.gameProps = props;
    if (props.targetFPS) {
        engine.targetFPS = *props.targetFPS;
        engine.updateInterval = 1000 / engine.targetFPS;
    }
    const uint32 fixedTargetFPS = props.targetFixedFPS ? *props.targetFixedFPS : 60;
    engine.fixedUpdateInterval = 1000 / fixedTargetFPS; // 16 ms per update when fixedUpdateInterval is 60
    engine.fixedDeltaTime = (f32)engine.fixedUpdateInterval / 1000.0f;
    return true;
}

void re_quit() {
    engine.isRunning = false;
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
    // Handle fixed updates
    accumulator += deltaTime;
    while (accumulator >= engine.fixedUpdateInterval) {
        engine_fixed_update();
        fpsTracker.fixedFPS++;
        accumulator -= engine.fixedUpdateInterval;
    }
    // Handle variable update
    const f32 deltaTimeSeconds = (f32)deltaTime / 1000.f;
    engine_update(deltaTimeSeconds);
    fpsTracker.FPS++;

    update_average_fps();

    if (engine.gameProps.targetFPS) {
        const uint64 frameTime = SDL_GetTicks() - currentTime;
        if (frameTime < engine.updateInterval) {
            SDL_Delay(engine.updateInterval - frameTime);
        }
    }
}

void update_average_fps() {
    static uint64 lastTime = 0;
    const uint64 currentTime = SDL_GetTicks();
    const uint64 elapsedTime = currentTime - lastTime;
    if (elapsedTime >= 1000) {
        ska_logger_message("FPS: %d", fpsTracker.FPS);
        ska_logger_message("FPS (fixed): %d", fpsTracker.fixedFPS);
        gameStats.averageFPS = fpsTracker.FPS;
        gameStats.averageFixedFPS = fpsTracker.fixedFPS;
        fpsTracker.FPS = 0;
        fpsTracker.fixedFPS = 0;
        const uint64 timeDelta = elapsedTime - 1000;
        lastTime = currentTime - timeDelta;
    }
}

void engine_update(f32 deltaTime) {}

void engine_fixed_update() {}

void re_render() {}

const REGameStats* re_get_stats() {
    return re_is_running() ? &gameStats : NULL;
}
