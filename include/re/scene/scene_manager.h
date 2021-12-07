#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <vector>
#include <unordered_map>

#include "scene.h"
#include "../ecs/entity/entity.h"
#include "../utils/logger.h"

class SceneManager {
  public:
    static SceneManager* GetInstance();
    void ChangeToEmptyScene();
    void ChangeToScene(const std::string& filePath);
    // TODO: Pass in a scene from ecs orchestrator
    void ChangeToScene(Scene* newScene);
    void AddRootNode(Entity rootEntity);
    void AddChildNode(Entity child, Entity parent);
    void DeleteNode(Entity entity);
    bool IsNodeInScene(Entity entity) const;

  private:
    Scene *currentScene = nullptr;
    Logger *logger = nullptr;

    SceneManager();
};

#endif //SCENE_MANAGER_H
