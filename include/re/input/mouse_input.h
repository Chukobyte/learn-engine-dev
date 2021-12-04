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
