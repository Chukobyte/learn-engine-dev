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
