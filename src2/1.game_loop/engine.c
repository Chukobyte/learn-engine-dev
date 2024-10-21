#include "engine.h"

#include <seika/defines.h>

typedef struct REEngine {
    bool isRunning;
} REEgine;

static REEgine engine = { .isRunning = false };

void re_run() {
    while (engine.isRunning) {}
}

