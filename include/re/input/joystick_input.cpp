#include "joystick_input.h"

JoystickInput::JoystickInput() : logger(Logger::GetInstance()) {}

JoystickInput::~JoystickInput() {
    SDL_JoystickClose(joystickController);
    SDL_GameControllerClose(gameController);
}

void JoystickInput::ProcessButtonPress(InputEvent &inputEvent) {
    const std::string &buttonValue = JOYSTICK_BUTTON_TYPE_TO_NAME_MAP[(JoystickButtonType) inputEvent.buttonValue];
    JOYSTICK_BUTTON_INPUT_FLAGS[buttonValue].isPressed = true;
    JOYSTICK_BUTTON_INPUT_FLAGS[buttonValue].isJustPressed = true;
}

void JoystickInput::ProcessButtonRelease(InputEvent &inputEvent) {
    const std::string &buttonValue = JOYSTICK_BUTTON_TYPE_TO_NAME_MAP[(JoystickButtonType) inputEvent.buttonValue];
    JOYSTICK_BUTTON_INPUT_FLAGS[buttonValue].isPressed = false;
    JOYSTICK_BUTTON_INPUT_FLAGS[buttonValue].isJustReleased = true;
}

void JoystickInput::ProcessJoyhatMotion(InputEvent &inputEvent) {
    if (inputEvent.hatValue & SDL_HAT_LEFT) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_LEFT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_LEFT].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_LEFT].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_LEFT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_LEFT].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_LEFT].isJustReleased = true;
        }
    }
    if (inputEvent.hatValue & SDL_HAT_RIGHT) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_RIGHT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_RIGHT].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_RIGHT].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_RIGHT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_RIGHT].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_RIGHT].isJustReleased = true;
        }
    }
    if (inputEvent.hatValue & SDL_HAT_UP) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_UP].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_UP].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_UP].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_UP].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_UP].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_UP].isJustReleased = true;
        }
    }
    if (inputEvent.hatValue & SDL_HAT_DOWN) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_DOWN].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_DOWN].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_DOWN].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_DOWN].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_DOWN].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_KEYPAD_DOWN].isJustReleased = true;
        }
    }
}

void JoystickInput::ProcessAxisMotion() {
    // LEFT AXIS
    // Horizontal
    Sint16 leftHorizontalValue = SDL_JoystickGetAxis(joystickController, (Uint8) JoystickAxisMotion::LEFT_HORIZONTAL_AXIS);
    if (leftHorizontalValue < -(Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_LEFT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_LEFT].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_LEFT].isJustPressed = true;
        }
    } else if (leftHorizontalValue > (Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_RIGHT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_RIGHT].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_RIGHT].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_LEFT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_LEFT].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_LEFT].isJustReleased = true;
        }
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_RIGHT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_RIGHT].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_RIGHT].isJustReleased = true;
        }
    }
    // Vertical
    Sint16 leftVerticalValue = SDL_JoystickGetAxis(joystickController, (Uint8) JoystickAxisMotion::LEFT_VERTICAL_AXIS);
    if (leftVerticalValue < -(Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_UP].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_UP].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_UP].isJustPressed = true;
        }
    } else if (leftVerticalValue > (Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_DOWN].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_DOWN].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_DOWN].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_UP].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_UP].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_UP].isJustReleased = true;
        }
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_DOWN].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_DOWN].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_ANALOG_DOWN].isJustReleased = true;
        }
    }

    // RIGHT AXIS
    // Horizontal
    Sint16 rightHorizontalValue = SDL_JoystickGetAxis(joystickController, (Uint8) JoystickAxisMotion::RIGHT_HORIZONTAL_AXIS);
    if (rightHorizontalValue < -(Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_LEFT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_LEFT].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_LEFT].isJustPressed = true;
        }
    } else if (rightHorizontalValue > (Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_RIGHT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_RIGHT].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_RIGHT].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_LEFT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_LEFT].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_LEFT].isJustReleased = true;
        }
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_RIGHT].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_RIGHT].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_RIGHT].isJustReleased = true;
        }
    }
    // Vertical
    Sint16 rightVerticalValue = SDL_JoystickGetAxis(joystickController, (Uint8) JoystickAxisMotion::RIGHT_VERTICAL_AXIS);
    if (rightVerticalValue < -(Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_UP].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_UP].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_UP].isJustPressed = true;
        }
    } else if (rightVerticalValue > (Uint8) JoystickDeadZone::AXIS) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_DOWN].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_DOWN].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_DOWN].isJustPressed = true;
        }
    } else {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_UP].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_UP].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_UP].isJustReleased = true;
        }
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_DOWN].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_DOWN].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_ANALOG_DOWN].isJustReleased = true;
        }
    }

    // Left Trigger
    Sint16 leftTriggerValue = SDL_JoystickGetAxis(joystickController, (Uint8) JoystickAxisMotion::LEFT_TRIGGER);
    if (leftTriggerValue < -(Uint8) JoystickDeadZone::TRIGGER) {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_TRIGGER].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_TRIGGER].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_TRIGGER].isJustReleased = true;
        }
    } else if (leftTriggerValue > (Uint8) JoystickDeadZone::TRIGGER) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_TRIGGER].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_TRIGGER].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_LEFT_TRIGGER].isJustPressed = true;
        }
    }
    // Right Trigger
    Sint16 rightTriggerValue = SDL_JoystickGetAxis(joystickController, (Uint8) JoystickAxisMotion::RIGHT_TRIGGER);
    if (rightTriggerValue < -(Uint8) JoystickDeadZone::TRIGGER) {
        if (JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_TRIGGER].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_TRIGGER].isPressed = false;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_TRIGGER].isJustReleased = true;
        }
    } else if (rightTriggerValue > (Uint8) JoystickDeadZone::TRIGGER) {
        if (!JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_TRIGGER].isPressed) {
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_TRIGGER].isPressed = true;
            JOYSTICK_BUTTON_INPUT_FLAGS[JOYSTICK_RIGHT_TRIGGER].isJustPressed = true;
        }
    }
}

JoystickInput* JoystickInput::GetInstance() {
    static JoystickInput *instance = new JoystickInput();
    return instance;
}

void JoystickInput::ProcessSDLEvent(InputEvent &inputEvent) {}

void JoystickInput::LoadJoysticks() {
    int result = SDL_GameControllerAddMappingsFromFile("assets/resources/game_controller_db.txt");
    assert(result != -1 && "Failed to load game controller db text file!");
    if (SDL_NumJoysticks() > 0) {
        joystickController = SDL_JoystickOpen(0);
        assert(joystickController != nullptr && "JoystickController didn't properly load!");
        gameController = SDL_GameControllerOpen(0);
        assert(gameController != nullptr && "GameController didn't properly load!");
    } else {
        logger->Warn("No joystick plugged in, not loading joysticks!");
    }
}

void JoystickInput::ClearInputFlags() {
    for (const auto &pair : JOYSTICK_BUTTON_INPUT_FLAGS) {
        JOYSTICK_BUTTON_INPUT_FLAGS[pair.first].isJustPressed = false;
        JOYSTICK_BUTTON_INPUT_FLAGS[pair.first].isJustReleased = false;
    }
}

bool JoystickInput::IsJoystickValue(const std::string &value) const {
    return false;
}

bool JoystickInput::IsActionPressed(const std::string &value) {
    return false;
}

bool JoystickInput::IsActionJustPressed(const std::string &value) {
    return false;
}

bool JoystickInput::IsActionJustReleased(const std::string &value) {
    return false;
}
