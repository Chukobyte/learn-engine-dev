#include "game_engine.h"

#include <SDL2/SDL.h>

#include "./re/math/redmath.h"

GameEngine::GameEngine() :
    projectProperties(ProjectProperties::GetInstance()),
    engineContext(GameEngineContext::GetInstance()),
    renderContext(RenderContext::GetInstance()),
    assetManager(AssetManager::GetInstance()),
    fpsCounter(FPSCounter::GetInstance()),
    logger(Logger::GetInstance()) {
    Initialize();
}

GameEngine::~GameEngine() {
    SDL_Quit();
    logger->Info("%s Engine has shut down!", engineContext->GetEngineName());
}

bool GameEngine::Initialize() {
    logger->Debug("Initializing...");
    if (!InitializeSDL()) {
        logger->Error("Failed to initialize SDL!");
        return false;
    }
    if (!InitializeRendering()) {
        logger->Error("Failed to initialize rendering!");
        return false;
    }
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);
    return true;
}

bool GameEngine::InitializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger->Error("Error on initializing SDL\n%s", SDL_GetError());
        return false;
    }
    return true;
}

bool GameEngine::InitializeRendering() {
    // OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    renderContext->window = SDL_CreateWindow(
                                projectProperties->GetGameTitle().c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                projectProperties->GetWindowWidth(),
                                projectProperties->GetWindowHeight(),
                                renderContext->windowFlags);
    renderContext->glContext = SDL_GL_CreateContext(renderContext->window);
    renderContext->currentWindowWidth = projectProperties->GetWindowWidth();
    renderContext->currentWindowHeight = projectProperties->GetWindowHeight();

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        logger->Error("Couldn't initialize glad");
        return false;
    }

    renderer2D.Initialize();

    // Temp Load Assets
    assetManager->LoadTexture("assets/images/melissa_walk_animation.png", "assets/images/melissa_walk_animation.png");
    return true;
}

void GameEngine::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            engineContext->SetRunning(false);
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                renderContext->currentWindowWidth = event.window.data1;
                renderContext->currentWindowHeight = event.window.data2;
                glViewport(0, 0, renderContext->currentWindowWidth, renderContext->currentWindowHeight);
                break;
            }
            break;
        }
    }
}

void GameEngine::Update() {
    // Sleep until FRAME_TARGET_TIME has elapsed since last frame
    const unsigned int MILLISECONDS_PER_TICK = 1000;
    const unsigned int TARGET_FPS = 60;
    static Uint32 lastFrameTime = 0;
    const unsigned int FRAME_TARGET_TIME = MILLISECONDS_PER_TICK / TARGET_FPS;
    unsigned int timeToWait = FRAME_TARGET_TIME - (SDL_GetTicks() - lastFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    fpsCounter->Update();

    lastFrameTime = SDL_GetTicks();
}

void GameEngine::Render() {
    glClearColor(projectProperties->backgroundClearColor.r,
                 projectProperties->backgroundClearColor.g,
                 projectProperties->backgroundClearColor.b,
                 projectProperties->backgroundClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    static const Vector2 windowCenter = Vector2(projectProperties->GetWindowWidth() / 2,
                                        projectProperties->GetWindowHeight() / 2);

    // Render Sprites
    static Texture *mellisaWalkTexture = assetManager->GetTexture("assets/images/melissa_walk_animation.png");
    static Rect2 drawSourceRect = Rect2(0, 0, 32, 32);
    static Rect2 drawDestinationRect = Rect2(windowCenter.x, windowCenter.y, drawSourceRect.w, drawSourceRect.h);
    renderer2D.SubmitSpriteBatchItem(mellisaWalkTexture, drawSourceRect, drawDestinationRect, 0);

    // Flush
    renderer2D.FlushBatches();


    SDL_GL_SwapWindow(renderContext->window);
}

bool GameEngine::IsRunning() const {
    return engineContext->IsRunning();
}
