#include "game_engine.h"

#include "audio_helper.h"
#include "./re/ecs/component/components/scene_component.h"
#include "./re/ecs/component/components/transform2d_component.h"
#include "./re/ecs/component/components/sprite_component.h"
#include "./re/ecs/component/components/text_label_component.h"
#include "./re/ecs/system/systems/sprite_rendering_ec_system.h"
#include "./re/ecs/system/systems/text_rendering_ec_system.h"

GameEngine::GameEngine() :
    projectProperties(ProjectProperties::GetInstance()),
    engineContext(GameEngineContext::GetInstance()),
    renderContext(RenderContext::GetInstance()),
    assetManager(AssetManager::GetInstance()),
    fpsCounter(FPSCounter::GetInstance()),
    renderer2D(Renderer2D::GetInstance()),
    logger(Logger::GetInstance()),
    inputManager(InputManager::GetInstance()),
    ecsOrchestrator(ECSOrchestrator::GetInstance()) {
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
    if (!InitializeECS()) {
        logger->Error("Failed to initialize ECS!");
        return false;
    }
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);

    // Temp play music
    AudioHelper::PlayMusic("test_music");
    return true;
}

bool GameEngine::InitializeSDL() {
    if (SDL_Init(SDL_INIT_EVERYTHING) != 0) {
        logger->Error("Error on initializing SDL\n%s", SDL_GetError());
        return false;
    }
    return true;
}

bool GameEngine::InitializeAudio() {
    if (Mix_OpenAudio(44100, MIX_DEFAULT_FORMAT, 2, 2048) < 0) {
        logger->Error("SDL_mixer could not be initialized!");
        return false;
    }

    // Temp load assets
    assetManager->LoadMusic("test_music", "assets/audio/music/test_music.wav");
    assetManager->LoadSound("test_sound", "assets/audio/sound/test_sound_effect.wav");
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

    renderContext->InitializeFont();
    renderer2D->Initialize();

    // Temp Load Assets
    assetManager->LoadTexture("assets/images/melissa_walk_animation.png", "assets/images/melissa_walk_animation.png");
    assetManager->LoadFont("assets/fonts/verdana.ttf", "assets/fonts/verdana.ttf", 20);
    return true;
}

bool GameEngine::InitializeInput() {
    inputManager->Initialize();
    // temp adding actions
    inputManager->AddAction("quit", "esc");
    inputManager->AddAction("move_left", "left");
    inputManager->AddAction("move_left", "a");
    inputManager->AddAction("move_right", "right");
    inputManager->AddAction("move_right", "d");
    return true;
}

bool GameEngine::InitializeECS() {
    // Register Components to ECS
    ecsOrchestrator->RegisterComponent<SceneComponent>();
    ecsOrchestrator->RegisterComponent<Transform2DComponent>();
    ecsOrchestrator->RegisterComponent<SpriteComponent>();
    ecsOrchestrator->RegisterComponent<TextLabelComponent>();
    // Register EC Systems to ECS
    ComponentSignature spriteRenderingSignature;
    spriteRenderingSignature.set(ecsOrchestrator->GetComponentType<Transform2DComponent>(), true);
    spriteRenderingSignature.set(ecsOrchestrator->GetComponentType<SpriteComponent>(), true);
    ecsOrchestrator->RegisterSystem<SpriteRenderingECSystem>(spriteRenderingSignature, ECSystemRegistration::RENDER);

    ComponentSignature textRenderingSignature;
    textRenderingSignature.set(ecsOrchestrator->GetComponentType<Transform2DComponent>(), true);
    textRenderingSignature.set(ecsOrchestrator->GetComponentType<TextLabelComponent>(), true);
    ecsOrchestrator->RegisterSystem<TextRenderingECSystem>(textRenderingSignature, ECSystemRegistration::RENDER);

    // Temp scene start up
    SceneManager* sceneManager = SceneManager::GetInstance();
    sceneManager->ChangeToEmptyScene();
    // Temp setup entities
    const Vector2 windowCenter = Vector2(projectProperties->GetWindowWidth() / 2,
                                         projectProperties->GetWindowHeight() / 2);
    // Main
    Entity mainEntity = ecsOrchestrator->CreateEntity();
    ecsOrchestrator->AddRootNode(mainEntity);
    // Sprite
    Entity witchEntity = ecsOrchestrator->CreateEntity();
    Transform2DComponent witchEntityTransform = Transform2DComponent{ windowCenter };
    ecsOrchestrator->AddComponent<Transform2DComponent>(witchEntity, witchEntityTransform);
    SpriteComponent witchEntitySpriteComponent = SpriteComponent{
        assetManager->GetTexture("assets/images/melissa_walk_animation.png"),
        Rect2(0, 0, 32, 32)
    };
    ecsOrchestrator->AddComponent<SpriteComponent>(witchEntity, witchEntitySpriteComponent);
    ecsOrchestrator->AddChildNode(witchEntity, mainEntity);
    // Text Label
    Entity textEntity = ecsOrchestrator->CreateEntity();
    Transform2DComponent textEntityTransform = Transform2DComponent{ Vector2(windowCenter.x - 35.0f, windowCenter.y - 20.0f) };
    ecsOrchestrator->AddComponent<Transform2DComponent>(textEntity, textEntityTransform);
    TextLabelComponent textEntityTextLabelComponent = TextLabelComponent{
        "Hello World",
        assetManager->GetFont("assets/fonts/verdana.ttf"),
        Color(1.0f, 1.0f, 1.0f)
    };
    ecsOrchestrator->AddComponent<TextLabelComponent>(textEntity, textEntityTextLabelComponent);
    ecsOrchestrator->AddChildNode(textEntity, mainEntity);
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
    inputManager->ProcessInputs(event);

    // Temp input processing
    if (inputManager->IsActionJustPressed("quit")) {
        engineContext->SetRunning(false);
    }
    // Temp moving left or right
    const bool moveLeftPressed = inputManager->IsActionPressed("move_left");
    const bool moveRightPressed = inputManager->IsActionPressed("move_right");
    if (moveLeftPressed || moveRightPressed) {
        const Entity witchEntity = 2;
        Transform2DComponent witchTransformComponent = ecsOrchestrator->GetComponent<Transform2DComponent>(witchEntity);
        witchTransformComponent.position.x += moveRightPressed ? 1 : -1;
        ecsOrchestrator->UpdateComponent<Transform2DComponent>(witchEntity, witchTransformComponent);
        SpriteComponent witchSpriteComponent = ecsOrchestrator->GetComponent<SpriteComponent>(witchEntity);
        witchSpriteComponent.flipX = !moveRightPressed;
        ecsOrchestrator->UpdateComponent<SpriteComponent>(witchEntity, witchSpriteComponent);
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

    inputManager->ClearInputFlags();

    ecsOrchestrator->DeleteEntitiesQueuedForDeletion();

    lastFrameTime = SDL_GetTicks();
}

void GameEngine::Render() {
    glClearColor(projectProperties->backgroundClearColor.r,
                 projectProperties->backgroundClearColor.g,
                 projectProperties->backgroundClearColor.b,
                 projectProperties->backgroundClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ecsOrchestrator->RenderSystems();

    // Flush
    renderer2D->FlushBatches();

    SDL_GL_SwapWindow(renderContext->window);
}

bool GameEngine::IsRunning() const {
    return engineContext->IsRunning();
}
