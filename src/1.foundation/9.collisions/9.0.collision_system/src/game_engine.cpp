#include "game_engine.h"

#include "./re/ecs/system/systems/sprite_rendering_ec_system.h"
#include "./re/ecs/system/systems/text_rendering_ec_system.h"
#include "./re/ecs/system/systems/animated_sprite_rendering_ec_system.h"
#include "./re/ecs/system/systems/collision_ec_system.h"
#include "./re/audio/audio_helper.h"
#include "./re/animation/animation_utils.h"
#include "./re/collision/collision_utils.h"

const Entity WITCH_ENTITY = 2;
const Entity WITCH_COLLIDER_ENTITY = 3;

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
    if (!LoadProjectProperties()) {
        logger->Error("Failed to load project properties!");
        return false;
    }
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
    assetManager->LoadProjectConfigurations(projectProperties->GetAssetConfigurations());
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);

    // Load initial scene
    ecsOrchestrator->PrepareSceneChange(projectProperties->GetInitialScenePath());

    // Temp play music
    AudioHelper::PlayMusic("assets/audio/music/test_music.wav");
    return true;
}

bool GameEngine::LoadProjectProperties() {
    const char* projectPropertiesPath = "properties.json";
    if (!FileHelper::DoesFileExist(projectPropertiesPath)) {
        logger->Error("Unable to find properties file '%s'", projectPropertiesPath);
        return false;
    }
    projectProperties->SetProjectProperties(JsonFileHelper::LoadJsonFile(projectPropertiesPath));
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

    return true;
}

bool GameEngine::InitializeInput() {
    inputManager->Initialize();
    inputManager->LoadInputActionConfigurations(projectProperties->GetInputActionsConfigurations());
    return true;
}

bool GameEngine::InitializeECS() {
    // Register Components to ECS
    ecsOrchestrator->RegisterComponent<SceneComponent>();
    ecsOrchestrator->RegisterComponent<Transform2DComponent>();
    ecsOrchestrator->RegisterComponent<SpriteComponent>();
    ecsOrchestrator->RegisterComponent<TextLabelComponent>();
    ecsOrchestrator->RegisterComponent<AnimatedSpriteComponent>();
    ecsOrchestrator->RegisterComponent<ColliderComponent>();
    // Register EC Systems to ECS
    ComponentSignature spriteRenderingSignature;
    spriteRenderingSignature.set(ecsOrchestrator->GetComponentType<Transform2DComponent>(), true);
    spriteRenderingSignature.set(ecsOrchestrator->GetComponentType<SpriteComponent>(), true);
    ecsOrchestrator->RegisterSystem<SpriteRenderingECSystem>(spriteRenderingSignature, ECSystemRegistration::RENDER);

    ComponentSignature textRenderingSignature;
    textRenderingSignature.set(ecsOrchestrator->GetComponentType<Transform2DComponent>(), true);
    textRenderingSignature.set(ecsOrchestrator->GetComponentType<TextLabelComponent>(), true);
    ecsOrchestrator->RegisterSystem<TextRenderingECSystem>(textRenderingSignature, ECSystemRegistration::RENDER);

    ComponentSignature animatedSpriteRenderingSignature;
    animatedSpriteRenderingSignature.set(ecsOrchestrator->GetComponentType<Transform2DComponent>(), true);
    animatedSpriteRenderingSignature.set(ecsOrchestrator->GetComponentType<AnimatedSpriteComponent>(), true);
    ecsOrchestrator->RegisterSystem<AnimatedSpriteRenderingECSystem>(animatedSpriteRenderingSignature, ECSystemRegistration::RENDER);

    ComponentSignature collisionSignature;
    collisionSignature.set(ecsOrchestrator->GetComponentType<Transform2DComponent>(), true);
    collisionSignature.set(ecsOrchestrator->GetComponentType<ColliderComponent>(), true);
    ECSystemRegistration colliderSystemRegistration = projectProperties->areColliderVisible ? ECSystemRegistration::RENDER : ECSystemRegistration::NONE;
    ecsOrchestrator->RegisterSystem<CollisionECSystem>(collisionSignature, colliderSystemRegistration);
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
        Transform2DComponent witchTransformComponent = ecsOrchestrator->GetComponent<Transform2DComponent>(WITCH_ENTITY);
        witchTransformComponent.position.x += moveRightPressed ? 1 : -1;
        ecsOrchestrator->UpdateComponent<Transform2DComponent>(WITCH_ENTITY, witchTransformComponent);
        AnimatedSpriteComponent witchAnimatedSpriteComponent = ecsOrchestrator->GetComponent<AnimatedSpriteComponent>(WITCH_ENTITY);
        witchAnimatedSpriteComponent.flipX = !moveRightPressed;
        ecsOrchestrator->UpdateComponent<AnimatedSpriteComponent>(WITCH_ENTITY, witchAnimatedSpriteComponent);
        AnimationUtils::PlayAnimation(WITCH_ENTITY, "walk");
    } else {
        AnimationUtils::PlayAnimation(WITCH_ENTITY, "idle");
    }
}

void GameEngine::PhysicsUpdate() {
    static Uint32 currentTime = SDL_GetTicks();
    static double accumulator = 0.0f;

    Uint32 newTime = SDL_GetTicks();
    Uint32 frameTime = newTime - currentTime;
    if (frameTime > Timing::Physics::MAX_FRAME_TIME) {
        frameTime = Timing::Physics::MAX_FRAME_TIME;
    }

    currentTime = newTime;
    accumulator += frameTime / static_cast<float>(Timing::Update::MILLISECONDS_PER_TICK);

    while (accumulator >= Timing::Physics::DELTA_TIME) {
        accumulator -= Timing::Physics::DELTA_TIME;
        ecsOrchestrator->PhysicsUpdateSystems(Timing::Physics::DELTA_TIME);
        inputManager->ClearInputFlags();
    }
}

void GameEngine::Update() {
    // Sleep until FRAME_TARGET_TIME has elapsed since last frame
    const Uint32 currentTime = SDL_GetTicks();
    static Uint32 lastFrameTime = 0;
    const unsigned int FRAME_TARGET_TIME = Timing::Update::MILLISECONDS_PER_TICK / projectProperties->GetTargetFPS();
    unsigned int timeToWait = FRAME_TARGET_TIME - (currentTime - lastFrameTime);
    if (timeToWait > 0 && timeToWait <= FRAME_TARGET_TIME) {
        SDL_Delay(timeToWait);
    }

    fpsCounter->Update();

    if (ecsOrchestrator->HasSceneToCreate()) {
        ecsOrchestrator->ChangeToScene();
        ecsOrchestrator->RegisterLoadedSceneNodeComponents();
        ecsOrchestrator->OnSceneStartSystems();
    }

    const float variableDeltaTime = (currentTime - lastFrameTime) / static_cast<float>(Timing::Update::MILLISECONDS_PER_TICK);
    ecsOrchestrator->UpdateSystems(variableDeltaTime);

    // temp collision check
    static bool hasCollidedPreviously = false;
    CollisionResult collisionResult = CollisionUtils::GetEntityCollisionResult(WITCH_COLLIDER_ENTITY);
    const bool hasCollided = !collisionResult.collidedEntities.empty();
    if (hasCollided && !hasCollidedPreviously) {
        logger->Debug("Colliders intersecting!");
    } else if (!hasCollided && hasCollidedPreviously) {
        logger->Debug("Colliders no longer intersecting!");
    }
    hasCollidedPreviously = hasCollided;

    PhysicsUpdate();

    inputManager->ClearInputFlags();

    if (ecsOrchestrator->HasSceneToDestroy()) {
        ecsOrchestrator->DestroyScene();
    }

    ecsOrchestrator->DestroyQueuedEntities();

    lastFrameTime = SDL_GetTicks();
}

void GameEngine::Render() {
    glClearColor(projectProperties->backgroundClearColor.r,
                 projectProperties->backgroundClearColor.g,
                 projectProperties->backgroundClearColor.b,
                 projectProperties->backgroundClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    ecsOrchestrator->RenderSystems();

    renderer2D->FlushBatches();

    SDL_GL_SwapWindow(renderContext->window);
}

bool GameEngine::IsRunning() const {
    return engineContext->IsRunning();
}
