#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

#include "input_action.h"
#include "mouse_input.h"
#include "joystick_input.h"
#include "input_event_state.h"
#include "../utils/project_properties.h"

class InputManager {
  public:
    static InputManager* GetInstance();
    void Initialize();
    void ProcessInputs(SDL_Event &event);
    void ClearInputFlags();
    void AddAction(const std::string &actionName, const std::string &actionValue);
    void RemoveAction(const std::string &actionName);
    bool IsActionPressed(const std::string &actionName);
    bool IsActionJustPressed(const std::string &actionName);
    bool IsActionJustReleased(const std::string &actionName);
    InputEvent GetCurrentInputEvent() const;
    void LoadInputActionConfigurations(InputActionsConfigurations inputActionsConfigurations);

  private:
    MouseInput *mouseInput = nullptr;
    JoystickInput *joystickInput = nullptr;
    InputEvent currentInputEvent;
    InputEventState inputEventState;
    std::unordered_map<std::string, InputAction*> inputActions;

    InputManager();
};


#endif //INPUT_MANAGER_H
