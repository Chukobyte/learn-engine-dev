#pragma once

#include "../utils/singleton.h"

#include <vector>

#include "../math/redmath.h"
#include "../ecs/component/component_manager.h"

struct CollisionResult {
    Entity sourceEntity = NULL_ENTITY;
    std::vector<Entity> collidedEntities;
};

class CollisionContext : public Singleton<CollisionContext> {
  public:
    CollisionContext(singleton);
    Rect2 GetCollisionRectangle(Entity entity);
    bool IsTargetCollisionEntityInExceptionList(Entity sourceEntity, Entity targetEntity);

  private:
    ComponentManager* componentManager = nullptr;

};
