# Input Management

## Input Action

```c++
#ifndef INPUT_ACTION_H
#define INPUT_ACTION_H

#include <vector>
#include <algorithm>

#include <SDL2/SDL.h>

#include "mouse_input.h"
#include "keyboard_input.h"
#include "joystick_input.h"

#include "../utils/logger.h"

class InputAction {
  private:
    MouseAction mouseAction;
    KeyboardAction keyboardAction;
    JoystickAction joystickAction;
    Logger *logger = nullptr;
  public:
    bool isActionPressed = false;
    bool isActionJustPressed = false;
    bool isActionJustReleased = false;

    InputAction() {
        logger = Logger::GetInstance();
    }

    void AddValue(const std::string &actionValue) {
        if (mouseAction.IsMouseValue(actionValue)) {
            mouseAction.AddValue(actionValue);
        } else if (joystickAction.IsJoystickValue(actionValue)) {
            joystickAction.AddValue(actionValue);
        } else {
            keyboardAction.AddValue(actionValue);
        }
    }

    bool IsActionPressed() {
        if (mouseAction.IsActionPressed()) {
            return true;
        }
        if (keyboardAction.isActionPressed) {
            return true;
        }
        if (joystickAction.IsActionPressed()) {
            return true;
        }
        return false;
    }

    bool IsActionJustPressed() {
        if (mouseAction.IsActionJustPressed()) {
            return true;
        }
        if (keyboardAction.isActionJustPressed) {
            return true;
        }
        if (joystickAction.IsActionJustPressed()) {
            return true;
        }
        return false;
    }

    bool IsActionJustReleased() {
        if (mouseAction.IsActionJustReleased()) {
            return true;
        }
        if (keyboardAction.isActionJustReleased) {
            return true;
        }
        if (joystickAction.IsActionJustReleased()) {
            return true;
        }
        return false;
    }

    void ProcessInputs(const Uint8* keyboardState) {
        keyboardAction.ProcessInputs(keyboardState);
    }

    void ClearInputFlags() {
        keyboardAction.isActionJustPressed = false;
        keyboardAction.isActionJustReleased = false;
    }

};

#endif //INPUT_ACTION_H
```

## Input Manager

```c++
#ifndef INPUT_MANAGER_H
#define INPUT_MANAGER_H

#include <unordered_map>

#include "input_action.h"
#include "mouse_input.h"
#include "joystick_input.h"
#include "input_event_state.h"

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

  private:
    MouseInput *mouseInput = nullptr;
    JoystickInput *joystickInput = nullptr;
    InputEvent currentInputEvent;
    InputEventState inputEventState;
    std::unordered_map<std::string, InputAction*> inputActions;

    InputManager();
};


#endif //INPUT_MANAGER_H
```

```c++
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
```

## Processing Input Events

Will add `inputManager` to `GameEngine` class.

```c++
InputManager *inputManager = nullptr;
```

Initialize `inputManager` in constructor.

```c++
GameEngine::GameEngine() :
    projectProperties(ProjectProperties::GetInstance()),
    engineContext(GameEngineContext::GetInstance()),
    renderContext(RenderContext::GetInstance()),
    assetManager(AssetManager::GetInstance()),
    fpsCounter(FPSCounter::GetInstance()),
    logger(Logger::GetInstance()),
    inputManager(InputManager::GetInstance()) {
    Initialize();
}
```

Adding new function `InitializeInput`.

```c++
bool GameEngine::InitializeInput() {
    inputManager->Initialize();
    // temp adding actions
    inputManager->AddAction("quit", "esc");
    inputManager->AddAction("play_sound", "space");
    return true;
}
```

Call `InitializeInput` in `Initialize` function.

```c++
bool GameEngine::Initialize() {
    logger->Debug("Initializing...");
    if (!InitializeSDL()) {
        logger->Error("Failed to initialize SDL!");
        return false;
    }
    if (!InitializeAudio()) {
        logger->Error("Failed to initialize audio!");
        return false;
    }
    if (!InitializeRendering()) {
        logger->Error("Failed to initialize rendering!");
        return false;
    }
    if (!InitializeInput()) {
        logger->Error("Failed to initialize input!");
        return false;
    }
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);

    // Temp play music
    AudioHelper::PlayMusic("test_music");
    return true;
}
```

Adding this to the bottom of the `ProcessInput` function.

```c++
inputManager->ProcessInputs(event);

// Temp input processing
if (inputManager->IsActionJustPressed("quit")) {
    engineContext->SetRunning(false);
}
if (inputManager->IsActionJustPressed("play_sound")) {
    AudioHelper::PlaySound("test_sound");
}
```

If you run the engine now, you'll be able to exit out of the game by pressing the ESC key.  You can also trigger a sound effect to play by pressing the SPACE key.  All source code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/5.input_management/5.0.input_management).

Now that we have input management in place, it's time to expand further and build out our Entity Component System.
