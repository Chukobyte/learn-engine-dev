#pragma once

#include <glad/glad.h>
#include <SDL2/SDL.h>
#include <ft2build.h>
#include FT_FREETYPE_H

class RenderContext {
  public:
    SDL_Window *window = nullptr;
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(
                                      SDL_WINDOW_OPENGL
                                      | SDL_WINDOW_RESIZABLE
                                      | SDL_WINDOW_ALLOW_HIGHDPI
                                  );
    SDL_GLContext glContext;
    FT_Library freeTypeLibrary;
    unsigned int currentWindowWidth;
    unsigned int currentWindowHeight;

    static RenderContext* GetInstance();
    void InitializeFont();

  private:
    RenderContext() = default;
};
