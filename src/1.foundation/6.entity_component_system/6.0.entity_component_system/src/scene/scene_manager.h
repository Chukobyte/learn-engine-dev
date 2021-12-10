#ifndef SCENE_MANAGER_H
#define SCENE_MANAGER_H

#include <vector>
#include <unordered_map>

#include "scene.h"
#include "./re/ecs/entity/entity.h"
#include "./re/utils/logger.h"

class SceneManager {
  public:
    static SceneManager* GetInstance();
    void ChangeToEmptyScene();
    void AddRootNode(Entity rootEntity);
    void AddChildNode(Entity child, Entity parent);
    void DeleteNode(Entity entity);
    bool IsNodeInScene(Entity entity) const;

  private:
    Scene *currentScene = nullptr;
    Logger *logger = nullptr;

    SceneManager() = default;
};

#endif //SCENE_MANAGER_H
