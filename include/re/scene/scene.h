#ifndef SCENE_H
#define SCENE_H

#include <cassert>

#include "../ecs/entity/entity.h"
#include "../ecs/component/components/scene_component.h"
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
