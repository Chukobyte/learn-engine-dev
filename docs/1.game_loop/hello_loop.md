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
    // Will limit internal fixed updates to match 'targetFPS'
    uint32* targetFPS;
    // Will limit the FPS of the entire engine update to match the 'targetFPS'
    bool limitFPS;
} REGameProperties;

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
```

The `REGameProperties` struct will contain user defined configuration for the game.  We will eventually add more properties to it as we build upon the engine.  Below the properties are the functions needed to start the engine, run the main loop, and quit the engine.  We will go into more detail within the implementation.

*engine.c*
```c
#include "engine.h"

#include <stdlib.h>

#include <SDL3/SDL.h>

#include <seika/logger.h>

// Represents an instance of the red engine
struct REEngine {
    bool isRunning;
    REGameProperties gameProps;
    uint64 targetFPS;
    uint64 fixedUpdateInterval;
    f32 fixedDeltaTime;
};

// FPS related stats
struct REFPSTracker {
    int32 FPS;
    int32 fixedFPS;
    int32 averageFPS;
    int32 averageFixedFPS;
};

// Will update the average fps
static void update_average_fps();
// Internal engine variable delta update
static void engine_update(f32 deltaTime);
// Internal engine fixed update
static void engine_fixed_update();

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
```

Now that we have define the logic for our engine instance and game loop, let's actually use it.

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
