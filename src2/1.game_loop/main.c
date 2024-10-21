#include <stdlib.h>

#include <seika/input/input.h>

int main(int argv, char** args) {
    ska_input_initialize();

    while (true) {
        if (ska_input_is_key_just_pressed(SkaInputKey_KEYBOARD_ESCAPE, 0)) {
            break;
        }
    }

    ska_input_finalize();

    return EXIT_SUCCESS;
}
