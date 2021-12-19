#include "collision_context.h"

#include "../ecs/component/components/collider_component.h"
#include "../ecs/component/components/transform2d_component.h"
#include "../scene/scene_node_utils.h"

CollisionContext::CollisionContext() : componentManager(ComponentManager::GetInstance()) {}

CollisionContext* CollisionContext::GetInstance() {
    static CollisionContext* instance = new CollisionContext();
    return instance;
}

Rect2 CollisionContext::GetCollisionRectangle(Entity entity) {
    ColliderComponent colliderComponent = componentManager->GetComponent<ColliderComponent>(entity);
    Transform2DComponent translatedTransform = SceneNodeUtils::TranslateEntityTransformIntoWorld(entity);
    return Rect2(translatedTransform.position.x + colliderComponent.collider.x,
                 translatedTransform.position.y + colliderComponent.collider.y,
                 translatedTransform.scale.x * colliderComponent.collider.w,
                 translatedTransform.scale.y * colliderComponent.collider.h);
}

bool CollisionContext::IsTargetCollisionEntityInExceptionList(Entity sourceEntity, Entity targetEntity) {
    ColliderComponent sourceColliderComponent = componentManager->GetComponent<ColliderComponent>(sourceEntity);
    return std::find(sourceColliderComponent.collisionExceptions.begin(), sourceColliderComponent.collisionExceptions.end(), targetEntity) != sourceColliderComponent.collisionExceptions.end();
}
