#ifndef SCENE_LOADER_H
#define SCENE_LOADER_H

#include "scene.h"

#include "../ecs/entity/entity_manager.h"
#include "../ecs/component/component_manager.h"
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

    void ParseComponents(SceneNode &sceneNode, const nlohmann::json &nodeComponentJsonArray) {
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

    void ParseTransform2DComponent(SceneNode &sceneNode, const nlohmann::json &nodeComponentObjectJson) {
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
        signature.set(componentManager->GetComponentType<Transform2DComponent>(), true);
        entityManager->SetSignature(sceneNode.entity, signature);
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }

    void ParseSpriteComponent(SceneNode &sceneNode, const nlohmann::json &nodeComponentObjectJson) {
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
        const bool isSpriteEnabled = !nodeTexturePath.empty();
        signature.set(componentManager->GetComponentType<SpriteComponent>(), isSpriteEnabled);
        entityManager->SetSignature(sceneNode.entity, signature);
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }

    void ParseTextLabelComponent(SceneNode &sceneNode, const nlohmann::json &nodeComponentObjectJson) {
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
        const bool isTextLabelEnabled = !nodeFontUID.empty();
        signature.set(componentManager->GetComponentType<TextLabelComponent>(), isTextLabelEnabled);
        entityManager->SetSignature(sceneNode.entity, signature);
        entityManager->SetEnabledSignature(sceneNode.entity, signature);
    }

  public:
    SceneNodeJsonParser() :
        entityManager(EntityManager::GetInstance()),
        componentManager(ComponentManager::GetInstance()),
        assetManager(AssetManager::GetInstance()) {}

    SceneNode ParseSceneJson(Scene* scene, nlohmann::json nodeJson, bool isRoot) {
        SceneNode sceneNode;
        if (isRoot) {
            sceneNode = {entityManager->CreateEntity()};
        } else {
            sceneNode = {entityManager->CreateEntity(), JsonHelper::Get<unsigned int>(nodeJson, "parent_entity_id")};
        }

        const std::string &nodeName = JsonHelper::Get<std::string>(nodeJson, "name");
        const std::string &nodeType = JsonHelper::Get<std::string>(nodeJson, "type");
        nlohmann::json nodeTagsJsonArray = JsonHelper::Get<nlohmann::json>(nodeJson, "tags");
        const std::string &nodeExternalSceneSource = JsonHelper::Get<std::string>(nodeJson, "external_scene_source");
        nlohmann::json nodeComponentJsonArray = JsonHelper::Get<nlohmann::json>(nodeJson, "components");
        nlohmann::json nodeChildrenJsonArray = JsonHelper::Get<nlohmann::json>(nodeJson, "children");

        // Configure node type component
//        std::vector<std::string> nodeTags;
//        for (const std::string &nodeTag : nodeTagsJsonArray) {
//            nodeTags.emplace_back(nodeTag);
//        }
//        componentManager->AddComponent(sceneNode.entity, NodeComponent{
//            .type = NodeTypeHelper::GetNodeTypeInt(nodeType),
//            .name = nodeName,
//            .tags = nodeTags
//        });

        // Rest of components
        ParseComponents(sceneNode, nodeComponentJsonArray);

        for (nlohmann::json nodeChildJson : nodeChildrenJsonArray) {
            nodeChildJson["parent_entity_id"] = sceneNode.entity;
            SceneNode childNode = ParseSceneJson(scene, nodeChildJson, false);
            sceneNode.children.emplace_back(childNode);
        }

        scene->sceneNodes.emplace(sceneNode.entity, sceneNode);
        if (isRoot) {
            scene->rootNode = sceneNode;
        }

        return sceneNode;
    }
};

class SceneLoader {
  public:
    static Scene* LoadSceneFile(const std::string& filePath) {
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

    static void AddNodesToScene(Scene* scene) {

    }
};

#endif //SCENE_LOADER_H
