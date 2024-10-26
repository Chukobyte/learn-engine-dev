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
