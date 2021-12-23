#pragma once

#include "../utils/singleton.h"

#include <vector>
#include <unordered_map>

#include "scene_loader.h"
#include "../ecs/entity/entity.h"
#include "../utils/logger.h"

class SceneManager : public Singleton<SceneManager>{
  public:
    SceneManager(singleton);
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
};
