#pragma once

#include <vector>

#include "../math/redmath.h"
#include "../ecs/component/component_manager.h"

struct CollisionResult {
    Entity sourceEntity = NULL_ENTITY;
    std::vector<Entity> collidedEntities;
};

class CollisionContext {
  public:
    static CollisionContext* GetInstance();
    Rect2 GetCollisionRectangle(Entity entity);
    bool IsTargetCollisionEntityInExceptionList(Entity sourceEntity, Entity targetEntity);

  private:
    CollisionContext();

    ComponentManager* componentManager = nullptr;

};
