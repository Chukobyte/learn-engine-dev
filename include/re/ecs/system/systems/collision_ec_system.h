#pragma once

#include "../ec_system.h"

#include "../../../math/physics.h"
#include "../../../collision/collision_context.h"
#include "../../../scene/scene_node_utils.h"
#include "../../component/components/transform2d_component.h"
#include "../../component/components/collider_component.h"
#include "../../../rendering/renderer_2d.h"

class CollisionECSystem : public ECSystem {
  public:
    CollisionECSystem() :
        collisionContext(CollisionContext::GetInstance()),
        renderer2D(Renderer2D::GetInstance()),
        componentManager(ComponentManager::GetInstance()) {
        collisionBaseTexture = new Texture(1, 1);
    }
    ~CollisionECSystem() {
        if (collisionBaseTexture) {
            delete collisionBaseTexture;
        }
    }

    void Render() override {
        if (IsEnabled()) {
            for (Entity entity : entities) {
                Transform2DComponent translatedTransform = SceneNodeUtils::TranslateEntityTransformIntoWorld(entity);
                ColliderComponent colliderComponent = componentManager->GetComponent<ColliderComponent>(entity);
                Vector2 drawDestinationSize = Vector2(colliderComponent.collider.w * translatedTransform.scale.x, colliderComponent.collider.h * translatedTransform.scale.y);
                Rect2 drawDestination = Rect2(translatedTransform.position, drawDestinationSize);
                static const Rect2 drawSourceRect = Rect2(0, 0, 1, 1);
                renderer2D->SubmitSpriteBatchItem(
                    collisionBaseTexture,
                    drawSourceRect,
                    drawDestination,
                    translatedTransform.zIndex,
                    translatedTransform.rotation,
                    colliderComponent.color
                );
            }
        }
    }

    CollisionResult GetEntityCollisionResult(Entity entity) {
        std::vector<Entity> collidedEntities = {};
        for (Entity targetEntity : entities) {
            if (entity == targetEntity) {
                continue;
            }
            if (!collisionContext->IsTargetCollisionEntityInExceptionList(entity, targetEntity)) {
                Rect2 sourceCollisionRectangle = collisionContext->GetCollisionRectangle(entity);
                Rect2 targetCollisionRectangle = collisionContext->GetCollisionRectangle(targetEntity);
                if (RedMath::Collision::AABB(sourceCollisionRectangle, targetCollisionRectangle)) {
                    collidedEntities.emplace_back(targetEntity);
                }
            }
        }
        return CollisionResult{
            entity,
            collidedEntities
        };
    }

  private:
    CollisionContext* collisionContext = nullptr;
    Renderer2D* renderer2D = nullptr;
    ComponentManager* componentManager = nullptr;
    Texture* collisionBaseTexture = nullptr;
};
