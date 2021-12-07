#ifndef SCENE_H
#define SCENE_H

#include "./re/ecs/entity/entity.h"

struct SceneNode {
    Entity entity = NULL_ENTITY;
    SceneNode *parent = nullptr;
    std::vector<SceneNode*> children;
};

struct Scene {
    SceneNode *rootNode = nullptr;
    std::unordered_map<Entity, SceneNode*> sceneNodes;
};

#endif //SCENE_H
