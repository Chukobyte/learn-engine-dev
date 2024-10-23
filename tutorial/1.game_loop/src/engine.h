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
