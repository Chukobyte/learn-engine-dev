#include <stdlib.h>

#include "engine.h"

int main(int argv, char** args) {
    re_run((REGameProperties){
        .name =  "lesson 1",
        .targetFPS = NULL,
        .delayCPU = false,
    });

    while (re_is_running()) {
        re_update();

        re_render();
    }

    return EXIT_SUCCESS;
}
