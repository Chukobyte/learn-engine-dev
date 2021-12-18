#pragma once

#include "./re/ecs/entity/entity_manager.h"
#include "./re/ecs/component/component_manager.h"
#include "./re/ecs/component/components/scene_component.h"
#include "./re/ecs/component/components/transform2d_component.h"
#include "./re/ecs/component/components/text_label_component.h"
#include "./re/ecs/component/components/sprite_component.h"
#include "./re/utils/file_helper.h"
#include "./re/utils/json_helper.h"

#include "../asset_manager.h"
#include "scene.h"

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
