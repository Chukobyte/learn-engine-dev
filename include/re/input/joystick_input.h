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
