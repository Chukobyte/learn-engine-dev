#ifndef RENDER_CONTEXT_H
#define RENDER_CONTEXT_H

#include <glad/glad.h>
#include <SDL2/SDL.h>

class RenderContext {
  private:
    RenderContext() = default;

  public:
    SDL_Window *window = nullptr;
    SDL_WindowFlags windowFlags = (SDL_WindowFlags)(
                                      SDL_WINDOW_OPENGL
                                      | SDL_WINDOW_RESIZABLE
                                      | SDL_WINDOW_ALLOW_HIGHDPI
                                  );
    SDL_GLContext glContext;

    static RenderContext* GetInstance();
};

#endif //RENDER_CONTEXT_H
