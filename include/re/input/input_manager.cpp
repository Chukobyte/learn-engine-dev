#include "input_manager.h"

InputManager::InputManager() : mouseInput(MouseInput::GetInstance()), joystickInput(JoystickInput::GetInstance()) {}

InputManager* InputManager::GetInstance() {
    static InputManager *instance = new InputManager();
    return instance;
}

void InputManager::Initialize() {
    joystickInput->LoadJoysticks();
}

void InputManager::ProcessInputs(SDL_Event &event) {
    currentInputEvent = inputEventState.ProcessSDLEvent(event);
    const Uint8* keyboardState = SDL_GetKeyboardState(nullptr);
    mouseInput->ProcessSDLEvent(currentInputEvent);
    joystickInput->ProcessSDLEvent(currentInputEvent);
    for (auto const &pair : inputActions) {
        InputAction *inputAction = pair.second;
        inputAction->ProcessInputs(keyboardState);
    }
}

void InputManager::ClearInputFlags() {
    mouseInput->ClearInputFlags();
    joystickInput->ClearInputFlags();
    for (auto const &pair : inputActions) {
        InputAction *inputAction = pair.second;
        inputAction->ClearInputFlags();
    }
}

void InputManager::AddAction(const std::string &actionName, const std::string &actionValue) {
    if (inputActions.count(actionName) <= 0) {
        inputActions.emplace(actionName, new InputAction());
    }
    inputActions[actionName]->AddValue(actionValue);
}

void InputManager::RemoveAction(const std::string &actionName) {
    if (inputActions.count(actionName) > 0) {
        delete inputActions[actionName];
        inputActions.erase(actionName);
    }
}

bool InputManager::IsActionPressed(const std::string &actionName) {
    if (inputActions.count(actionName) > 0) {
        return inputActions[actionName]->IsActionPressed();
    }
    return false;
}

bool InputManager::IsActionJustPressed(const std::string &actionName) {
    if (inputActions.count(actionName) > 0) {
        return inputActions[actionName]->IsActionJustPressed();
    }
    return false;
}

bool InputManager::IsActionJustReleased(const std::string &actionName) {
    if (inputActions.count(actionName) > 0) {
        return inputActions[actionName]->IsActionJustReleased();
    }
    return false;
}

InputEvent InputManager::GetCurrentInputEvent() const {
    return currentInputEvent;
}
