# Input Events

## Input Event State

```c++
#ifndef INPUT_EVENT_STATE_H
#define INPUT_EVENT_STATE_H

#include <SDL2/SDL.h>

#include "../math/math.h"

enum class InputEventType : std::uint32_t {
    MOUSE = 2,
    KEYBOARD = 4,
    JOYSTICK = 8,
};

struct InputEvent {
    InputEventType type;
    Uint32 sdlType;
    // Mouse
    Vector2 mouseMotion;
    bool mousePressed = false;
    Uint8 mouseButton;
    // Joystick
    Uint8 controllerId;
    Sint16 buttonValue;
    bool buttonPressed = false;
    Uint8 hat;
    Uint8 hatValue;
    // Keyboard
    bool keyPressed = false;
    SDL_Scancode keyScancode;
};

class InputEventState {
  public:
    InputEvent ProcessSDLEvent(SDL_Event &event) {
        InputEvent inputEvent;
        inputEvent.sdlType = event.type;
        switch(event.type) {
        // Mouse
        case SDL_MOUSEMOTION:
            inputEvent.type = InputEventType::MOUSE;
            inputEvent.mouseMotion = Vector2(event.motion.x, event.motion.y);
            break;
        case SDL_MOUSEWHEEL:
            inputEvent.type = InputEventType::MOUSE;
            break;
        case SDL_MOUSEBUTTONDOWN:
            inputEvent.type = InputEventType::MOUSE;
            inputEvent.mousePressed = true;
            inputEvent.mouseButton = event.button.button;
            break;
        case SDL_MOUSEBUTTONUP:
            inputEvent.type = InputEventType::MOUSE;
            inputEvent.mousePressed = false;
            inputEvent.mouseButton = event.button.button;
            break;
        // Joystick
        case SDL_JOYBUTTONDOWN:
        case SDL_JOYBUTTONUP:
            inputEvent.type = InputEventType::JOYSTICK;
            inputEvent.controllerId = event.jbutton.which;
            inputEvent.buttonValue = event.jbutton.button;
            inputEvent.buttonPressed = event.jbutton.state == SDL_PRESSED;
            break;
        case SDL_JOYHATMOTION:
            inputEvent.type = InputEventType::JOYSTICK;
            inputEvent.controllerId = event.jhat.which;
            inputEvent.hat = event.jhat.hat;
            inputEvent.hatValue = event.jhat.value;
            break;
        // Keyboard
        case SDL_KEYDOWN:
        case SDL_KEYUP:
            if (!event.key.repeat) {
                inputEvent.type = InputEventType::KEYBOARD;
                inputEvent.keyPressed = event.key.state == SDL_PRESSED;
                inputEvent.keyScancode = event.key.keysym.scancode;
            }
            break;
        default:
            break;
        }
        return inputEvent;
    }
};

#endif //INPUT_EVENT_STATE_H

```

## Keyboard

```c++
#ifndef KEYBOARD_INPUT_H
#define KEYBOARD_INPUT_H

#include <string>
#include <map>

#include <SDL2/SDL.h>

const std::string KEYBOARD_KEY_UP{"up"};
const std::string KEYBOARD_KEY_DOWN{"down"};
const std::string KEYBOARD_KEY_LEFT{"left"};
const std::string KEYBOARD_KEY_RIGHT{"right"};
const std::string KEYBOARD_KEY_SPACE{"space"};
const std::string KEYBOARD_KEY_ESC{"esc"};
const std::string KEYBOARD_KEY_RETURN{"return"};

static std::map<std::string, SDL_Scancode> KEYBOARD_SCANCODE_TO_VALUE_MAPPING = {
    {"0", SDL_SCANCODE_0},
    {"1", SDL_SCANCODE_1},
    {"2", SDL_SCANCODE_2},
    {"3", SDL_SCANCODE_3},
    {"4", SDL_SCANCODE_4},
    {"5", SDL_SCANCODE_5},
    {"6", SDL_SCANCODE_6},
    {"7", SDL_SCANCODE_7},
    {"8", SDL_SCANCODE_8},
    {"9", SDL_SCANCODE_9},

    {"a", SDL_SCANCODE_A},
    {"b", SDL_SCANCODE_B},
    {"c", SDL_SCANCODE_C},
    {"d", SDL_SCANCODE_D},
    {"e", SDL_SCANCODE_E},
    {"f", SDL_SCANCODE_F},
    {"g", SDL_SCANCODE_G},
    {"h", SDL_SCANCODE_H},
    {"i", SDL_SCANCODE_I},
    {"j", SDL_SCANCODE_J},
    {"k", SDL_SCANCODE_K},
    {"l", SDL_SCANCODE_L},
    {"m", SDL_SCANCODE_M},
    {"n", SDL_SCANCODE_N},
    {"o", SDL_SCANCODE_O},
    {"p", SDL_SCANCODE_P},
    {"q", SDL_SCANCODE_Q},
    {"r", SDL_SCANCODE_R},
    {"s", SDL_SCANCODE_S},
    {"t", SDL_SCANCODE_T},
    {"u", SDL_SCANCODE_U},
    {"v", SDL_SCANCODE_V},
    {"w", SDL_SCANCODE_W},
    {"x", SDL_SCANCODE_X},
    {"y", SDL_SCANCODE_Y},
    {"z", SDL_SCANCODE_Z},

    {KEYBOARD_KEY_LEFT, SDL_SCANCODE_LEFT},
    {KEYBOARD_KEY_RIGHT, SDL_SCANCODE_RIGHT},
    {KEYBOARD_KEY_UP, SDL_SCANCODE_UP},
    {KEYBOARD_KEY_DOWN, SDL_SCANCODE_DOWN},

    {KEYBOARD_KEY_RETURN, SDL_SCANCODE_RETURN},
    {KEYBOARD_KEY_SPACE, SDL_SCANCODE_SPACE},
    {KEYBOARD_KEY_ESC, SDL_SCANCODE_ESCAPE},

    {"f1", SDL_SCANCODE_F1},
    {"f2", SDL_SCANCODE_F2},
    {"f3", SDL_SCANCODE_F3},
    {"f4", SDL_SCANCODE_F4},
    {"f5", SDL_SCANCODE_F5},
    {"f6", SDL_SCANCODE_F6},
    {"f7", SDL_SCANCODE_F7},
    {"f8", SDL_SCANCODE_F8},
    {"f9", SDL_SCANCODE_F9},
    {"f10", SDL_SCANCODE_F10},
    {"f11", SDL_SCANCODE_F11},
    {"f12", SDL_SCANCODE_F12}
};

class KeyboardAction {
  public:
    bool isActionPressed = false;
    bool isActionJustPressed = false;
    bool isActionJustReleased = false;
    std::vector<SDL_Scancode> values;
    SDL_Scancode lastScancodePressed = SDL_SCANCODE_UNKNOWN;

    void AddValue(const std::string &actionValue) {
        if (KEYBOARD_SCANCODE_TO_VALUE_MAPPING.count(actionValue) > 0) {
            SDL_Scancode valueScancode = KEYBOARD_SCANCODE_TO_VALUE_MAPPING[actionValue];
            values.emplace_back(valueScancode);
        }
    }

    void ProcessInputs(const Uint8* keyboardState) {
        for(SDL_Scancode scancodeValue : values) {
            bool isKeyboardValuePressed = keyboardState[scancodeValue];
            if (isKeyboardValuePressed && !isActionPressed) {
                isActionPressed = true;
                isActionJustPressed = true;
                lastScancodePressed = scancodeValue;
                break;
            } else if (!isKeyboardValuePressed && !isActionJustReleased && isActionPressed && scancodeValue == lastScancodePressed) {
                isActionPressed = false;
                isActionJustPressed = false;
                isActionJustReleased = true;
            }
        }
    }
};

#endif //KEYBOARD_INPUT_H
```

## Mouse

```c++
#ifndef MOUSE_INPUT_H
#define MOUSE_INPUT_H

#include <string>
#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "../math/math.h"

#include "input_event_state.h"

const std::string MOUSE_BUTTON_LEFT{"mb_left"};
const std::string MOUSE_BUTTON_RIGHT{"mb_right"};
const std::string MOUSE_BUTTON_MIDDLE{"mb_middle"};

class MouseInput {
  public:
    bool isMouseLeftButtonPressed = false;
    bool isMouseLeftButtonJustPressed = false;
    bool isMouseLeftButtonJustReleased = false;
    bool isMouseRightButtonPressed = false;
    bool isMouseRightButtonJustPressed = false;
    bool isMouseRightButtonJustReleased = false;

    static MouseInput* GetInstance();
    void ClearInputFlags();
    Vector2 GetMousePosition() const;
    void ProcessSDLEvent(InputEvent &inputEvent);

  private:
    static MouseInput *instance;
    Vector2 mousePosition;

    MouseInput() {}
};

class MouseAction {
  public:
    MouseAction() : mouseInput(MouseInput::GetInstance()) {}

    static bool IsMouseValue(const std::string &actionValue) {
        if (actionValue == MOUSE_BUTTON_LEFT || actionValue == MOUSE_BUTTON_RIGHT) {
            return true;
        }
        return false;
    }

    void AddValue(const std::string &actionValue) {
        if (!HasValue(actionValue)) {
            mouseValues.emplace_back(actionValue);
        }
    }

    bool HasValue(const std::string &actionValue) {
        return std::find(mouseValues.begin(), mouseValues.end(), actionValue) != mouseValues.end();
    }

    bool IsActionPressed() {
        for (const std::string &value : mouseValues) {
            if ((value == MOUSE_BUTTON_LEFT && mouseInput->isMouseLeftButtonPressed)
                    || (value == MOUSE_BUTTON_RIGHT && mouseInput->isMouseRightButtonPressed)) {
                return true;
            }
        }
        return false;
    }

    bool IsActionJustPressed() {
        for (const std::string &value : mouseValues) {
            if ((value == MOUSE_BUTTON_LEFT && mouseInput->isMouseLeftButtonJustPressed)
                    || (value == MOUSE_BUTTON_RIGHT && mouseInput->isMouseRightButtonJustPressed)) {
                return true;
            }
        }
        return false;
    }

    bool IsActionJustReleased() {
        for (const std::string &value : mouseValues) {
            if ((value == MOUSE_BUTTON_LEFT && mouseInput->isMouseLeftButtonJustReleased)
                    || (value == MOUSE_BUTTON_RIGHT && mouseInput->isMouseRightButtonJustReleased)) {
                return true;
            }
        }
        return false;
    }

  private:
    std::vector<std::string> mouseValues;
    MouseInput *mouseInput = nullptr;
};

#endif //MOUSE_INPUT_H
```

```c++
#include "mouse_input.h"

MouseInput* MouseInput::GetInstance() {
    static MouseInput *instance = new MouseInput();
    return instance;
}

void MouseInput::ClearInputFlags() {
    isMouseLeftButtonJustPressed = false;
    isMouseRightButtonJustPressed = false;
    isMouseLeftButtonJustReleased = false;
    isMouseRightButtonJustReleased = false;
}

Vector2 MouseInput::GetMousePosition() const {
    return mousePosition;
}

void MouseInput::ProcessSDLEvent(InputEvent &inputEvent) {
    switch(inputEvent.sdlType) {
    // Mouse
    case SDL_MOUSEMOTION:
        mousePosition = inputEvent.mouseMotion;
        break;
    case SDL_MOUSEWHEEL:
        break;
    case SDL_MOUSEBUTTONDOWN:
        if(inputEvent.mouseButton == SDL_BUTTON_LEFT) {
            isMouseLeftButtonPressed = true;
            isMouseLeftButtonJustPressed = true;
        } else if(inputEvent.mouseButton == SDL_BUTTON_RIGHT) {
            isMouseRightButtonPressed = true;
            isMouseRightButtonJustPressed = true;
        }
        break;
    case SDL_MOUSEBUTTONUP:
        if(inputEvent.mouseButton == SDL_BUTTON_LEFT) {
            isMouseLeftButtonPressed = false;
            isMouseLeftButtonJustPressed = false;
            isMouseLeftButtonJustReleased = true;
        } else if(inputEvent.mouseButton == SDL_BUTTON_RIGHT) {
            isMouseRightButtonPressed = false;
            isMouseRightButtonJustPressed = false;
            isMouseRightButtonJustReleased = true;
        }
        break;
    default:
        break;
    }
}
```

## Joystick

```c++
#ifndef JOYSTICK_INPUT_H
#define JOYSTICK_INPUT_H

#include <string>
#include <vector>
#include <unordered_map>
#include <algorithm>

#include <SDL2/SDL.h>

#include "input_event_state.h"
#include "../utils/logger.h"

enum class JoystickButtonType : int {
    INVALID = -1,
    BUTTON_A = 0,
    BUTTON_B = 1,
    BUTTON_X = 2,
    BUTTON_Y = 3,
    BUTTON_BACK = 6,
    BUTTON_START = 7,
    BUTTON_LEFT_SHOULDER = 4,
    BUTTON_RIGHT_SHOULDER = 5,
    BUTTON_LEFT_AXIS = 8,
    BUTTON_RIGHT_AXIS = 9,
};

enum class JoystickDeadZone : int {
    AXIS = 10000,
    TRIGGER = 8000,
};

enum class JoystickAxisMotion : Uint8 {
    LEFT_TRIGGER = 2,
    RIGHT_TRIGGER = 5,
    LEFT_HORIZONTAL_AXIS = 0,
    LEFT_VERTICAL_AXIS = 1,
    RIGHT_HORIZONTAL_AXIS = 3,
    RIGHT_VERTICAL_AXIS = 4,
};

const std::string JOYSTICK_BUTTON_A{"joystick_button_a"};
const std::string JOYSTICK_BUTTON_B{"joystick_button_b"};
const std::string JOYSTICK_BUTTON_X{"joystick_button_x"};
const std::string JOYSTICK_BUTTON_Y{"joystick_button_y"};
const std::string JOYSTICK_KEYPAD_UP{"joystick_keypad_up"};
const std::string JOYSTICK_KEYPAD_DOWN{"joystick_keypad_down"};
const std::string JOYSTICK_KEYPAD_RIGHT{"joystick_keypad_right"};
const std::string JOYSTICK_KEYPAD_LEFT{"joystick_keypad_left"};
const std::string JOYSTICK_START{"joystick_button_start"};
const std::string JOYSTICK_BACK{"joystick_button_back"};
const std::string JOYSTICK_LEFT_SHOULDER{"joystick_left_shoulder"};
const std::string JOYSTICK_RIGHT_SHOULDER{"joystick_right_shoulder"};
const std::string JOYSTICK_LEFT_TRIGGER{"joystick_left_trigger"};
const std::string JOYSTICK_RIGHT_TRIGGER{"joystick_right_trigger"};
const std::string JOYSTICK_LEFT_ANALOG{"joystick_left_analog"};
const std::string JOYSTICK_RIGHT_ANALOG{"joystick_right_analog"};
const std::string JOYSTICK_LEFT_ANALOG_UP{"joystick_left_analog_up"};
const std::string JOYSTICK_LEFT_ANALOG_DOWN{"joystick_left_analog_down"};
const std::string JOYSTICK_LEFT_ANALOG_RIGHT{"joystick_left_analog_right"};
const std::string JOYSTICK_LEFT_ANALOG_LEFT{"joystick_left_analog_left"};
const std::string JOYSTICK_RIGHT_ANALOG_UP{"joystick_right_analog_up"};
const std::string JOYSTICK_RIGHT_ANALOG_DOWN{"joystick_right_analog_down"};
const std::string JOYSTICK_RIGHT_ANALOG_RIGHT{"joystick_right_analog_right"};
const std::string JOYSTICK_RIGHT_ANALOG_LEFT{"joystick_right_analog_left"};

static std::unordered_map<std::string, JoystickButtonType> JOYSTICK_NAME_TO_BUTTON_TYPE_MAP = {
    {JOYSTICK_BUTTON_A, JoystickButtonType::BUTTON_A},
    {JOYSTICK_BUTTON_B, JoystickButtonType::BUTTON_B},
    {JOYSTICK_BUTTON_X, JoystickButtonType::BUTTON_X},
    {JOYSTICK_BUTTON_Y, JoystickButtonType::BUTTON_Y},
    {JOYSTICK_START, JoystickButtonType::BUTTON_START},
    {JOYSTICK_BACK, JoystickButtonType::BUTTON_BACK},
    {JOYSTICK_LEFT_SHOULDER, JoystickButtonType::BUTTON_LEFT_SHOULDER},
    {JOYSTICK_RIGHT_SHOULDER, JoystickButtonType::BUTTON_RIGHT_SHOULDER},
    {JOYSTICK_LEFT_ANALOG, JoystickButtonType::BUTTON_LEFT_AXIS},
    {JOYSTICK_RIGHT_ANALOG, JoystickButtonType::BUTTON_RIGHT_AXIS},
};

static std::unordered_map<JoystickButtonType, std::string> JOYSTICK_BUTTON_TYPE_TO_NAME_MAP = {
    {JoystickButtonType::BUTTON_A, JOYSTICK_BUTTON_A},
    {JoystickButtonType::BUTTON_B, JOYSTICK_BUTTON_B},
    {JoystickButtonType::BUTTON_X, JOYSTICK_BUTTON_X},
    {JoystickButtonType::BUTTON_Y, JOYSTICK_BUTTON_Y},
    {JoystickButtonType::BUTTON_START, JOYSTICK_START},
    {JoystickButtonType::BUTTON_BACK, JOYSTICK_BACK},
    {JoystickButtonType::BUTTON_LEFT_SHOULDER, JOYSTICK_LEFT_SHOULDER},
    {JoystickButtonType::BUTTON_RIGHT_SHOULDER, JOYSTICK_RIGHT_SHOULDER},
    {JoystickButtonType::BUTTON_LEFT_AXIS, JOYSTICK_LEFT_ANALOG},
    {JoystickButtonType::BUTTON_RIGHT_AXIS, JOYSTICK_RIGHT_ANALOG},
};

static std::unordered_map<std::string, SDL_GameControllerButton> JOYSTICK_ACTION_BUTTON_MAPPINGS = {
    {JOYSTICK_BUTTON_A, SDL_CONTROLLER_BUTTON_A},
    {JOYSTICK_BUTTON_B, SDL_CONTROLLER_BUTTON_B},
    {JOYSTICK_BUTTON_X, SDL_CONTROLLER_BUTTON_X},
    {JOYSTICK_BUTTON_Y, SDL_CONTROLLER_BUTTON_Y},
    {JOYSTICK_KEYPAD_UP, SDL_CONTROLLER_BUTTON_DPAD_UP},
    {JOYSTICK_KEYPAD_DOWN, SDL_CONTROLLER_BUTTON_DPAD_DOWN},
    {JOYSTICK_KEYPAD_LEFT, SDL_CONTROLLER_BUTTON_DPAD_LEFT},
    {JOYSTICK_KEYPAD_RIGHT, SDL_CONTROLLER_BUTTON_DPAD_RIGHT},
    {JOYSTICK_START, SDL_CONTROLLER_BUTTON_START},
    {JOYSTICK_BACK, SDL_CONTROLLER_BUTTON_BACK},
    {JOYSTICK_LEFT_SHOULDER, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {JOYSTICK_RIGHT_SHOULDER, SDL_CONTROLLER_BUTTON_RIGHTSHOULDER},
    {JOYSTICK_LEFT_SHOULDER, SDL_CONTROLLER_BUTTON_LEFTSHOULDER},
    {JOYSTICK_LEFT_ANALOG, SDL_CONTROLLER_BUTTON_LEFTSTICK},
    {JOYSTICK_RIGHT_ANALOG, SDL_CONTROLLER_BUTTON_RIGHTSTICK},
    // Non Game Controller Button Actions
    {JOYSTICK_LEFT_TRIGGER, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_RIGHT_TRIGGER, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_LEFT_ANALOG_UP, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_LEFT_ANALOG_DOWN, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_LEFT_ANALOG_RIGHT, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_LEFT_ANALOG_LEFT, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_RIGHT_ANALOG_UP, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_RIGHT_ANALOG_DOWN, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_RIGHT_ANALOG_RIGHT, SDL_CONTROLLER_BUTTON_INVALID},
    {JOYSTICK_RIGHT_ANALOG_LEFT, SDL_CONTROLLER_BUTTON_INVALID}
};

struct JoystickInputPressState {
    bool isPressed = false;
    bool isJustPressed = false;
    bool isJustReleased = false;
};

static std::unordered_map<std::string, JoystickInputPressState> JOYSTICK_BUTTON_INPUT_FLAGS = {
    // Button Process
    {JOYSTICK_BUTTON_B,           JoystickInputPressState{}},
    {JOYSTICK_BUTTON_A,           JoystickInputPressState{}},
    {JOYSTICK_BUTTON_X,           JoystickInputPressState{}},
    {JOYSTICK_BUTTON_Y,           JoystickInputPressState{}},
    {JOYSTICK_START,              JoystickInputPressState{}},
    {JOYSTICK_BACK,               JoystickInputPressState{}},
    {JOYSTICK_LEFT_SHOULDER,      JoystickInputPressState{}},
    {JOYSTICK_RIGHT_SHOULDER,     JoystickInputPressState{}},
    {JOYSTICK_LEFT_ANALOG,        JoystickInputPressState{}},
    {JOYSTICK_RIGHT_ANALOG,       JoystickInputPressState{}},
    // Hat Process
    {JOYSTICK_KEYPAD_LEFT,        JoystickInputPressState{}},
    {JOYSTICK_KEYPAD_RIGHT,       JoystickInputPressState{}},
    {JOYSTICK_KEYPAD_UP,          JoystickInputPressState{}},
    {JOYSTICK_KEYPAD_DOWN,        JoystickInputPressState{}},
    // Axis Process
    {JOYSTICK_LEFT_ANALOG_LEFT,   JoystickInputPressState{}},
    {JOYSTICK_LEFT_ANALOG_RIGHT,  JoystickInputPressState{}},
    {JOYSTICK_LEFT_ANALOG_UP,     JoystickInputPressState{}},
    {JOYSTICK_LEFT_ANALOG_DOWN,   JoystickInputPressState{}},
    {JOYSTICK_RIGHT_ANALOG_LEFT,  JoystickInputPressState{}},
    {JOYSTICK_RIGHT_ANALOG_RIGHT, JoystickInputPressState{}},
    {JOYSTICK_RIGHT_ANALOG_UP,    JoystickInputPressState{}},
    {JOYSTICK_RIGHT_ANALOG_DOWN,  JoystickInputPressState{}},
    {JOYSTICK_LEFT_TRIGGER,       JoystickInputPressState{}},
    {JOYSTICK_RIGHT_TRIGGER,      JoystickInputPressState{}},
};

class JoystickInput {
  public:
    static JoystickInput* GetInstance();
    void ProcessSDLEvent(InputEvent &inputEvent);
    void LoadJoysticks();
    void ClearInputFlags();
    bool IsJoystickValue(const std::string &value) const;
    bool IsActionPressed(const std::string &value);
    bool IsActionJustPressed(const std::string &value);
    bool IsActionJustReleased(const std::string &value);

  private:
    SDL_Joystick *joystickController = nullptr;
    SDL_GameController *gameController = nullptr;
    Logger *logger = nullptr;

    JoystickInput();
    ~JoystickInput();
    void ProcessButtonPress(InputEvent &inputEvent);
    void ProcessButtonRelease(InputEvent &inputEvent);
    void ProcessJoyhatMotion(InputEvent &inputEvent);
    void ProcessAxisMotion();
};

class JoystickAction {
  public:
    JoystickAction() : joystickInput(JoystickInput::GetInstance()) {}

    bool IsJoystickValue(const std::string &actionValue) {
        return joystickInput->IsJoystickValue(actionValue);
    }

    void AddValue(const std::string &actionValue) {
        if (!HasValue(actionValue)) {
            joystickValues.emplace_back(actionValue);
        }
    }

    bool HasValue(const std::string &actionValue) {
        return std::find(joystickValues.begin(), joystickValues.end(), actionValue) != joystickValues.end();
    }

    bool IsActionPressed() {
        for (const std::string &value : joystickValues) {
            if (joystickInput->IsActionPressed(value)) {
                return true;
            }
        }
        return false;
    }

    bool IsActionJustPressed() {
        for (const std::string &value : joystickValues) {
            if (joystickInput->IsActionJustPressed(value)) {
                return true;
            }
        }
        return false;
    }

    bool IsActionJustReleased() {
        for (const std::string &value : joystickValues) {
            if (joystickInput->IsActionJustReleased(value)) {
                return true;
            }
        }
        return false;
    }

  private:
    JoystickInput *joystickInput = nullptr;
    std::vector<std::string> joystickValues;
};

#endif //JOYSTICK_INPUT_H
```

```c++
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
```
