#pragma once

#include <vector>
#include <unordered_map>

#include "./re/ecs/entity/entity.h"
#include "./re/utils/helper.h"

struct SceneNode {
    Entity entity = NULL_ENTITY;
    Entity parent = NULL_ENTITY;
    std::vector<SceneNode> children = {};
};

struct Scene {
    SceneNode rootNode = {};
    std::unordered_map<Entity, SceneNode> sceneNodes = {};
};
