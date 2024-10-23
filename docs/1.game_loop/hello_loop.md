# Hello Loop

## What is a game loop?

The very first things we are going to create for our game engine is a game loop.  But what is a game loop?  A game loop is the core control mechanism of a game engine that continuously runs throughout the life the game.  The game loop is responsible for updating the game state, processing inputs, rendering graphics to the screen among other things.

*engine.h*
```c
#pragma once

#include <seika/defines.h>

typedef struct REGameProperties {
    const char* name;
    uint32* targetFPS;
    bool limitFPS;
} REGameProperties;

bool re_run(REGameProperties props);
bool re_is_running();
void re_update();
void re_render();
```

*engine.c*
```c
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
