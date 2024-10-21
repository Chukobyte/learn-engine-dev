# Hello Loop

## What is a game loop?

The very first things we are going to create for our game engine is a game loop.  But what is a game loop?

engine.h
```c
#pragma once

void re_run();
```

engine.c
```c
#include "engine.h"

#include <seika/defines.h>

typedef struct REEngine {
    bool isRunning;
} REEgine;

static REEgine engine = { .isRunning = false };

void re_run() {
    while (engine.isRunning) {}
}
```

main.c
```c
#include <stdlib.h>

#include "engine.h"

int main(int argv, char** args) {
    re_run();
    return EXIT_SUCCESS;
}
```
