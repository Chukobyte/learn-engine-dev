#ifndef SCENE_H
#define SCENE_H

#include <vector>
#include <unordered_map>

#include "../ecs/entity/entity.h"
#include "../utils/helper.h"

struct SceneNode {
    Entity entity = NULL_ENTITY;
    Entity parent = NULL_ENTITY;
    std::vector<SceneNode> children = {};
};

struct Scene {
    SceneNode rootNode = {};
    std::unordered_map<Entity, SceneNode> sceneNodes = {};
};

#endif //SCENE_H
