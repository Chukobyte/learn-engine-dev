# Hello Loop

## What is a game loop?

The very first things we are going to create for our game engine is a game loop.  But what is a game loop?  A game loop is the core control mechanism of a game engine that continuously runs throughout the life the game.  The game loop is responsible for updating the game state, processing inputs, rendering graphics to the screen among other things.

With that being said, let's define the logic for the game loop.

*engine.h*
```c
#pragma once

#include <seika/defines.h>

// Game related properties
typedef struct REGameProperties {
    // The name of the game
    const char* name;
    // (Optional) Will limit internal updates to match 'targetFPS'
    uint32* targetFPS;
    // (Optional) Will limit internal fixed updates to match 'targetFixedFPS'
    uint32* targetFixedFPS;
} REGameProperties;

// Game related stats
typedef struct REGameStats {
    // Average frames per second
    int32 averageFPS;
    // Average fixed frames per second
    int32 averageFixedFPS;
} REGameStats;

// Will run the engine with the passed in properties
bool re_run(REGameProperties props);
// Will quit the engine
void re_quit();
// Returns true if the engine is running
bool re_is_running();
// Applies updates to internal state related things such as input, audio, timing, etc
void re_update();
// Applies updates needed for rendering and will render what's available
void re_render();
// Will return a readonly `REGameStats` object.  `NULL` is returned if the engine isn't running.
const REGameStats* re_get_stats();
```

The `REGameProperties` struct contains user defined configuration for the game.  The `REGameStats` struct will hold game engine related stats, such as FPS. We will eventually add more properties and stats to both structs respectably as we add more to the engine.  Below the struct definitions are the functions needed to start the engine, run the main loop, and quit the engine.  We will go into more detail within the implementation.

*engine.c*
```c
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
    uint32 targetFPS;
    // Actual update interval for the game loop
    uint32 updateInterval;
    // The update interval used when doing an internal fixed update
    uint32 fixedUpdateInterval;
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
        ska_logger_message("FPS: %d\nFPS (fixed): %d", fpsTracker.FPS, fpsTracker.fixedFPS);
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
```

Now that we have defined the logic for our engine instance and game loop, let's actually use it.

*main.c*
```c
#include <stdlib.h>

#include "engine.h"

int main(int argv, char** args) {
    re_run((REGameProperties){
        .name =  "lesson 1",
        .targetFPS = NULL,
        .limitFPS = false,
    });

    while (re_is_running()) {
        re_update();
        re_render();
    }

    return EXIT_SUCCESS;
}
```
