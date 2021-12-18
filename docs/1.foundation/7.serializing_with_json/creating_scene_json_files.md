# Creating Scene Json Files

## Scene Loader

```c++
#pragma once

#include "scene.h"

#include "../ecs/entity/entity_manager.h"
#include "../ecs/component/component_manager.h"
#include "../ecs/component/components/scene_component.h"
#include "../ecs/component/components/transform2d_component.h"
#include "../ecs/component/components/text_label_component.h"
#include "../ecs/component/components/sprite_component.h"
#include "../data/asset_manager.h"
#include "../utils/file_helper.h"
#include "../utils/json_helper.h"

class SceneNodeJsonParser {
  private:
    EntityManager *entityManager = nullptr;
    ComponentManager *componentManager = nullptr;
    AssetManager *assetManager = nullptr;

    unsigned int GetEntityNameCount(const std::string& name, const SceneNode& parentSceneNode);
    std::string GetUniqueSceneNodeName(const std::string& name, const SceneNode& parentSceneNode);
    SceneComponent GenerateSceneComponent(const std::string& nodeName, const SceneNode& parentSceneNode, const nlohmann::json& nodeTagsJsonArray);
    void ParseComponentArray(SceneNode &sceneNode, const nlohmann::json& nodeComponentJsonArray);
    void ParseTransform2DComponent(SceneNode &sceneNode, const nlohmann::json& nodeComponentObjectJson);
    void ParseSpriteComponent(SceneNode &sceneNode, const nlohmann::json& nodeComponentObjectJson);
    void ParseTextLabelComponent(SceneNode& sceneNode, const nlohmann::json& nodeComponentObjectJson);

  public:
    SceneNodeJsonParser() :
        entityManager(EntityManager::GetInstance()),
        componentManager(ComponentManager::GetInstance()),
        assetManager(AssetManager::GetInstance()) {}

    SceneNode ParseSceneJson(Scene* scene, const nlohmann::json& nodeJson, bool isRoot, const SceneNode& parentSceneNode = {});
};

class SceneLoader {
  public:
    static Scene* LoadSceneFile(const std::string& filePath);
};
```

```c++
#include "scene_loader.h"

unsigned int SceneNodeJsonParser::GetEntityNameCount(const std::string& name, const SceneNode& parentSceneNode) {
    unsigned int enitityNameCount = 0;
    for (const SceneNode& childrenSceneNode : parentSceneNode.children) {
        SceneComponent sceneComponent = componentManager->GetComponent<SceneComponent>(childrenSceneNode.entity);
        std::string childName = sceneComponent.name;
        const std::string& childNumberAtTheEndString = Helper::GetNumberFromEndOfString(childName);
        if (childNumberAtTheEndString.empty()) {
            continue;
        }
        childName.resize(childName.size() - childNumberAtTheEndString.size());
        unsigned int childNumberAtEnd = Helper::ConvertStringToUnsignedInt(childNumberAtTheEndString);
        enitityNameCount = std::max(enitityNameCount, childNumberAtEnd);
    }
    return enitityNameCount;
}

std::string SceneNodeJsonParser::GetUniqueSceneNodeName(const std::string& name, const SceneNode& parentSceneNode) {
    if (!parentSceneNode.children.empty()) {
        unsigned int entitiesWithSameNameCount = GetEntityNameCount(name, parentSceneNode);
        if (entitiesWithSameNameCount > 0) {
            unsigned int uniqueId = entitiesWithSameNameCount + 1;
            const std::string& numberAtEndString = Helper::GetNumberFromEndOfString(name);
            std::string nameWithoutNumber = Helper::RemoveNumberFromEndOfString(name);
            // Make sure provided number isn't higher than unique id
            if (!numberAtEndString.empty()) {
                unsigned int numberAtTheEnd = Helper::ConvertStringToUnsignedInt(numberAtEndString);
                uniqueId = std::max(uniqueId, numberAtTheEnd);
            }
            return nameWithoutNumber + std::to_string(uniqueId);
        }
    }
    return name;
}

SceneComponent SceneNodeJsonParser::GenerateSceneComponent(const std::string& nodeName, const SceneNode& parentSceneNode, const nlohmann::json& nodeTagsJsonArray) {
    std::vector<std::string> nodeTags = {};
    for (auto& nodeTag : nodeTagsJsonArray) {
        nodeTags.emplace_back(nodeTag);
    }
    return SceneComponent{
        GetUniqueSceneNodeName(nodeName, parentSceneNode),
        nodeTags
    };
}

void SceneNodeJsonParser::ParseComponentArray(SceneNode &sceneNode, const nlohmann::json& nodeComponentJsonArray) {
    for (nlohmann::json nodeComponentJson : nodeComponentJsonArray) {
        nlohmann::json::iterator it = nodeComponentJson.begin();
        const std::string &nodeComponentType = it.key();
        nlohmann::json nodeComponentObjectJson = it.value();
        // TODO: Map to functions with keys
        if (nodeComponentType == "transform2D") {
            ParseTransform2DComponent(sceneNode, nodeComponentObjectJson);
        } else if (nodeComponentType == "sprite") {
            ParseSpriteComponent(sceneNode, nodeComponentObjectJson);
        } else if (nodeComponentType == "text_label") {
            ParseTextLabelComponent(sceneNode, nodeComponentObjectJson);
        }
    }
}

void SceneNodeJsonParser::ParseTransform2DComponent(SceneNode &sceneNode, const nlohmann::json& nodeComponentObjectJson) {
    nlohmann::json nodeTransform2DPosition = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "position");
    nlohmann::json nodeTransform2DScale = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "scale");
    const Vector2 nodePosition = Vector2(
                                     JsonHelper::Get<float>(nodeTransform2DPosition, "x"),
                                     JsonHelper::Get<float>(nodeTransform2DPosition, "y"));
    const Vector2 nodeScale = Vector2(
                                  JsonHelper::Get<float>(nodeTransform2DScale, "x"),
                                  JsonHelper::Get<float>(nodeTransform2DScale, "y"));
    const float nodeRotation = JsonHelper::Get<float>(nodeComponentObjectJson, "rotation");
    const int nodeZIndex = JsonHelper::Get<int>(nodeComponentObjectJson, "z_index");
    const bool nodeZIndexIsRelativeToParent = JsonHelper::Get<bool>(nodeComponentObjectJson, "z_index_relative_to_parent");
    const bool nodeIgnoreCamera = JsonHelper::Get<bool>(nodeComponentObjectJson, "ignore_camera");

    componentManager->AddComponent(sceneNode.entity, Transform2DComponent{
        .position = nodePosition,
        .scale = nodeScale,
        .rotation = nodeRotation,
        .zIndex = nodeZIndex,
        .isZIndexRelativeToParent = nodeZIndexIsRelativeToParent,
        .ignoreCamera = nodeIgnoreCamera
    });
    auto signature = entityManager->GetEnabledSignature(sceneNode.entity);
    bool isTransformComponentEnabled = JsonHelper::GetDefault<bool>(nodeComponentObjectJson, "enabled", true);
    signature.set(componentManager->GetComponentType<Transform2DComponent>(), true);
    entityManager->SetSignature(sceneNode.entity, signature);
    if (isTransformComponentEnabled) {
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }
}

void SceneNodeJsonParser::ParseSpriteComponent(SceneNode &sceneNode, const nlohmann::json& nodeComponentObjectJson) {
    const std::string &nodeTexturePath = JsonHelper::Get<std::string>(nodeComponentObjectJson, "texture_path");
    nlohmann::json nodeDrawSourceJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "draw_source");
    const float nodeDrawSourceX = JsonHelper::Get<float>(nodeDrawSourceJson, "x");
    const float nodeDrawSourceY = JsonHelper::Get<float>(nodeDrawSourceJson, "y");
    const float nodeDrawSourceWidth = JsonHelper::Get<float>(nodeDrawSourceJson, "width");
    const float nodeDrawSourceHeight = JsonHelper::Get<float>(nodeDrawSourceJson, "height");
    const bool nodeFlipX = JsonHelper::Get<bool>(nodeComponentObjectJson, "flip_x");
    const bool nodeFlipY = JsonHelper::Get<bool>(nodeComponentObjectJson, "flip_x");
    nlohmann::json nodeModulateJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "modulate");
    const Color nodeModulate = Color::NormalizedColor(
                                   JsonHelper::Get<int>(nodeModulateJson, "red"),
                                   JsonHelper::Get<int>(nodeModulateJson, "green"),
                                   JsonHelper::Get<int>(nodeModulateJson, "blue"),
                                   JsonHelper::Get<int>(nodeModulateJson, "alpha")
                               );

    componentManager->AddComponent(sceneNode.entity, SpriteComponent{
        .texture = nodeTexturePath.empty() ? nullptr : assetManager->GetTexture(nodeTexturePath),
        .drawSource = Rect2(nodeDrawSourceX, nodeDrawSourceY, nodeDrawSourceWidth, nodeDrawSourceHeight),
        .flipX = nodeFlipX,
        .flipY = nodeFlipY,
        .modulate = nodeModulate
    });
    auto signature = entityManager->GetEnabledSignature(sceneNode.entity);
    signature.set(componentManager->GetComponentType<SpriteComponent>(), true);
    entityManager->SetSignature(sceneNode.entity, signature);
    const bool isSpriteEnabled = !nodeTexturePath.empty();
    bool isSpriteComponentEnabled = JsonHelper::GetDefault<bool>(nodeComponentObjectJson, "enabled", true);
    if (isSpriteEnabled && isSpriteComponentEnabled) {
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }
}

void SceneNodeJsonParser::ParseTextLabelComponent(SceneNode& sceneNode, const nlohmann::json& nodeComponentObjectJson) {
    const std::string &nodeText = JsonHelper::Get<std::string>(nodeComponentObjectJson, "text");
    const std::string &nodeFontUID = JsonHelper::Get<std::string>(nodeComponentObjectJson, "font_uid");
    nlohmann::json nodeColorJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "color");
    const Color nodeColor = Color::NormalizedColor(
                                JsonHelper::Get<int>(nodeColorJson, "red"),
                                JsonHelper::Get<int>(nodeColorJson, "green"),
                                JsonHelper::Get<int>(nodeColorJson, "blue"),
                                JsonHelper::Get<int>(nodeColorJson, "alpha")
                            );

    componentManager->AddComponent(sceneNode.entity, TextLabelComponent{
        .text = nodeText,
        .font = nodeFontUID.empty() ? nullptr : assetManager->GetFont(nodeFontUID),
        .color = nodeColor
    });
    auto signature = entityManager->GetEnabledSignature(sceneNode.entity);
    signature.set(componentManager->GetComponentType<TextLabelComponent>(), true);
    entityManager->SetSignature(sceneNode.entity, signature);
    const bool isTextLabelEnabled = !nodeFontUID.empty();
    bool isTextLabelComponentEnabled = JsonHelper::GetDefault<bool>(nodeComponentObjectJson, "enabled", true);
    if (isTextLabelEnabled && isTextLabelComponentEnabled) {
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }
}

SceneNode SceneNodeJsonParser::ParseSceneJson(Scene* scene, const nlohmann::json& nodeJson, bool isRoot, const SceneNode& parentSceneNode) {
    SceneNode sceneNode;
    if (parentSceneNode.entity != NULL_ENTITY) {
        sceneNode = { entityManager->CreateEntity(), parentSceneNode.entity };
    } else {
        sceneNode = { entityManager->CreateEntity() };
    }

    // Configure scene component
    const std::string &nodeName = JsonHelper::Get<std::string>(nodeJson, "name");
    const std::string &nodeType = JsonHelper::Get<std::string>(nodeJson, "type");
    nlohmann::json nodeTagsJsonArray = JsonHelper::Get<nlohmann::json>(nodeJson, "tags");
    const std::string &nodeExternalSceneSource = JsonHelper::Get<std::string>(nodeJson, "external_scene_source");
    componentManager->AddComponent(sceneNode.entity, GenerateSceneComponent(nodeName, parentSceneNode, nodeTagsJsonArray));

    // Rest of components
    nlohmann::json nodeComponentJsonArray = JsonHelper::Get<nlohmann::json>(nodeJson, "components");
    ParseComponentArray(sceneNode, nodeComponentJsonArray);

    nlohmann::json nodeChildrenJsonArray = JsonHelper::Get<nlohmann::json>(nodeJson, "children");
    for (nlohmann::json nodeChildJson : nodeChildrenJsonArray) {
        SceneNode childNode = ParseSceneJson(scene, nodeChildJson, false, sceneNode);
        sceneNode.children.emplace_back(childNode);
    }

    scene->sceneNodes.emplace(sceneNode.entity, sceneNode);
    if (isRoot) {
        scene->rootNode = sceneNode;
    }

    return sceneNode;
}


Scene* SceneLoader::LoadSceneFile(const std::string& filePath) {
    Scene* loadedScene = new Scene();
    if (FileHelper::DoesFileExist(filePath)) {
        nlohmann::json sceneJson = JsonFileHelper::LoadJsonFile(filePath);
        static SceneNodeJsonParser sceneNodeJsonParser;
        sceneNodeJsonParser.ParseSceneJson(loadedScene, sceneJson, true);
    } else {
        Logger::GetInstance()->Error("Scene file '%s' not found!", filePath.c_str());
    }
    return loadedScene;
}
```

## More Changes to the Game Engine Class

Will update the `Update` function to account for scene changes.

```c++
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

    PhysicsUpdate();

    inputManager->ClearInputFlags();

    if (ecsOrchestrator->HasSceneToDestroy()) {
        ecsOrchestrator->DestroyScene();
    }

    ecsOrchestrator->DestroyQueuedEntities();

    lastFrameTime = SDL_GetTicks();
}
```

Update the `Initialize` function to change to initial scene.

```c++
bool GameEngine::Initialize() {
    // Initialize stuff above
    ...
    assetManager->LoadProjectConfigurations(projectProperties->GetAssetConfigurations());
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);

    // Load initial scene
    ecsOrchestrator->PrepareSceneChange(projectProperties->GetInitialScenePath());

    // Temp play music
    AudioHelper::PlayMusic("assets/audio/music/test_music.wav");
    return true;
}
```

If we now run ***Red Engine*** it should work as expected with no changes.  The source code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/7.serializing_with_json/7.1.creating_scene_json_files).  Now that we have scene loading with json it's time to start defining our engine api as well as implement more functionality.
