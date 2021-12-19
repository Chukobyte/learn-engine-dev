#pragma once

#include "../ecs/ecs_orchestrator.h"
#include "../ecs/system/systems/collision_ec_system.h"

class CollisionUtils {
  public:
    static CollisionResult GetEntityCollisionResult(Entity entity) {
        static CollisionECSystem* collisionSystem = ECSOrchestrator::GetInstance()->GetSystem<CollisionECSystem>();
        return collisionSystem->GetEntityCollisionResult(entity);
    }
};
