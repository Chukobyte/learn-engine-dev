#pragma once

#include <vector>
#include <unordered_map>

#include "scene_loader.h"
#include "./re/ecs/entity/entity.h"
#include "./re/utils/logger.h"

class SceneManager {
  public:
    static SceneManager* GetInstance();
    void ChangeToEmptyScene();
    void ChangeToScene(const std::string& filePath);
    void AddRootNode(Entity rootEntity);
    void AddChildNode(Entity child, Entity parent);
    void DeleteNode(Entity entity);
    bool IsNodeInScene(Entity entity) const;
    Scene* GetCurrentScene();

  private:
    Scene *currentScene = nullptr;
    Logger *logger = nullptr;

    SceneManager();
};