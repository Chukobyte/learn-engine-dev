#pragma once

#include <vector>
#include <unordered_map>
#include <cassert>

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

    SceneNode GetSceneNode(Entity entity) {
        assert(sceneNodes.count(entity) > 0 && "Entity doesn't have a scene node!");
        return sceneNodes[entity];
    }
};
