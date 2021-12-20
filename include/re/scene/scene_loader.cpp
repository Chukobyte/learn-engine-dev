#include "scene_loader.h"

#include <cassert>

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
        } else if (nodeComponentType == "animated_sprite") {
            ParseAnimatedSpriteComponent(sceneNode, nodeComponentObjectJson);
        } else if (nodeComponentType == "collider") {
            ParseColliderComponent(sceneNode, nodeComponentObjectJson);
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

void SceneNodeJsonParser::ParseAnimatedSpriteComponent(SceneNode& sceneNode, const nlohmann::json& nodeComponentObjectJson) {
    const std::string& currentAnimationName = JsonHelper::Get<std::string>(nodeComponentObjectJson, "current_animation");
    const bool isPlaying = JsonHelper::Get<bool>(nodeComponentObjectJson, "is_playing");
    const bool flipX = JsonHelper::Get<bool>(nodeComponentObjectJson, "flip_x");
    const bool flipY = JsonHelper::Get<bool>(nodeComponentObjectJson, "flip_y");
    const nlohmann::json& modulateJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "modulate");
    const Color modulateColor = Color::NormalizedColor(
                                    JsonHelper::Get<int>(modulateJson, "red"),
                                    JsonHelper::Get<int>(modulateJson, "green"),
                                    JsonHelper::Get<int>(modulateJson, "blue"),
                                    JsonHelper::Get<int>(modulateJson, "alpha")
                                );
    const nlohmann::json& animationsJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "animations");

    // Setup Animations
    static AssetManager* assetManager = AssetManager::GetInstance();
    std::unordered_map<std::string, Animation> nodeAnimations = {};
    for (const nlohmann::json& animationJson : animationsJson) {
        const std::string& nodeAnimationName = JsonHelper::Get<std::string>(animationJson, "name");
        const int nodeAnimationSpeed = JsonHelper::Get<int>(animationJson, "speed");
        nlohmann::json nodeAnimationFramesJsonArray = JsonHelper::Get<nlohmann::json>(animationJson, "frames");
        std::unordered_map<unsigned int, AnimationFrame> animationFrames;
        for (nlohmann::json nodeAnimationFrameJson : nodeAnimationFramesJsonArray) {
            const int nodeAnimationFrameNumber = JsonHelper::Get<int>(nodeAnimationFrameJson, "frame");
            const std::string &nodeAnimationTexturePath = JsonHelper::Get<std::string>(nodeAnimationFrameJson, "texture_path");
            nlohmann::json nodeAnimationFrameDrawSourceJson = JsonHelper::Get<nlohmann::json>(nodeAnimationFrameJson, "draw_source");
            const float nodeAnimationFrameDrawSourceX = JsonHelper::Get<float>(nodeAnimationFrameDrawSourceJson, "x");
            const float nodeAnimationFrameDrawSourceY = JsonHelper::Get<float>(nodeAnimationFrameDrawSourceJson, "y");
            const float nodeAnimationFrameDrawSourceWidth = JsonHelper::Get<float>(nodeAnimationFrameDrawSourceJson, "width");
            const float nodeAnimationFrameDrawSourceHeight = JsonHelper::Get<float>(nodeAnimationFrameDrawSourceJson, "height");
            AnimationFrame nodeAnimationFrame = {
                .texture = assetManager->GetTexture(nodeAnimationTexturePath),
                .drawSource = Rect2(nodeAnimationFrameDrawSourceX, nodeAnimationFrameDrawSourceY, nodeAnimationFrameDrawSourceWidth, nodeAnimationFrameDrawSourceHeight),
                .frame = nodeAnimationFrameNumber
            };
            animationFrames.emplace(nodeAnimationFrame.frame, nodeAnimationFrame);
        }

        Animation nodeAnimation = {
            .name = nodeAnimationName,
            .speed = nodeAnimationSpeed,
            .animationFrames = animationFrames,
            .frames = static_cast<unsigned int>(animationFrames.size())
        };
        nodeAnimations.emplace(nodeAnimation.name, nodeAnimation);
    }

    assert(nodeAnimations.count(currentAnimationName) > 0 && "Trying to set current animation to an animation that doesn't exist!");
    componentManager->AddComponent(sceneNode.entity, AnimatedSpriteComponent{
        nodeAnimations,
        nodeAnimations[currentAnimationName],
        isPlaying,
        flipX,
        flipY,
        modulateColor
    });

    auto signature = entityManager->GetEnabledSignature(sceneNode.entity);
    signature.set(componentManager->GetComponentType<AnimatedSpriteComponent>(), true);
    entityManager->SetSignature(sceneNode.entity, signature);
    bool isAnimatedSpriteComponentEnabled = JsonHelper::GetDefault<bool>(nodeComponentObjectJson, "enabled", true);
    if (isAnimatedSpriteComponentEnabled) {
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }
}

void SceneNodeJsonParser::ParseColliderComponent(SceneNode& sceneNode, const nlohmann::json& nodeComponentObjectJson) {
    const nlohmann::json& rectangleJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "rectangle");
    const float rectX = JsonHelper::Get<float>(rectangleJson, "x");
    const float rectY = JsonHelper::Get<float>(rectangleJson, "y");
    const float rectWidth = JsonHelper::Get<float>(rectangleJson, "width");
    const float rectHeight = JsonHelper::Get<float>(rectangleJson, "height");
    Rect2 colliderRect = Rect2(rectX, rectY, rectWidth, rectHeight);
    const nlohmann::json& colorJson = JsonHelper::Get<nlohmann::json>(nodeComponentObjectJson, "color");
    const Color colliderColor = Color::NormalizedColor(
                                    JsonHelper::Get<int>(colorJson, "red"),
                                    JsonHelper::Get<int>(colorJson, "green"),
                                    JsonHelper::Get<int>(colorJson, "blue"),
                                    JsonHelper::Get<int>(colorJson, "alpha")
                                );

    componentManager->AddComponent(sceneNode.entity, ColliderComponent{
        colliderRect,
        colliderColor
    });

    auto signature = entityManager->GetEnabledSignature(sceneNode.entity);
    signature.set(componentManager->GetComponentType<ColliderComponent>(), true);
    entityManager->SetSignature(sceneNode.entity, signature);
    bool isColliderComponentEnabled = JsonHelper::GetDefault<bool>(nodeComponentObjectJson, "enabled", true);
    if (isColliderComponentEnabled) {
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
    auto signature = entityManager->GetEnabledSignature(sceneNode.entity);
    signature.set(componentManager->GetComponentType<SceneComponent>(), true);
    entityManager->SetSignature(sceneNode.entity, signature);
    entityManager->SetEnabledSignature(sceneNode.entity, signature);

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
