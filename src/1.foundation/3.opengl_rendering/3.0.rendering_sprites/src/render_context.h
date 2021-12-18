#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>

class RenderContext {
  public:
    SDL_Window *window = nullptr;
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(
                                      SDL_WINDOW_OPENGL
                                      | SDL_WINDOW_RESIZABLE
                                      | SDL_WINDOW_ALLOW_HIGHDPI
                                  );
    SDL_GLContext glContext;
    unsigned int currentWindowWidth;
    unsigned int currentWindowHeight;

    static RenderContext* GetInstance();

  private:
    RenderContext() = default;
};
