#pragma once

#include <functional>

#include "scene_manager.h"
#include "../camera/camera_manager.h"
#include "../ecs/component/component_manager.h"
#include "../ecs/component/components/scene_component.h"
#include "../ecs/component/components/transform2d_component.h"
#include "re/math/redmath.h"

using CombineTransform2DFunction = std::function<Transform2DComponent(const Transform2DComponent& transformA, const Transform2DComponent& transformB)>;

class SceneNodeUtils {
  public:
    static Transform2DComponent GetEntityCombinedParentsTransforms(Entity entity) {
        static SceneManager* sceneManager = SceneManager::GetInstance();
        static ComponentManager* componentManager = ComponentManager::GetInstance();
        static CombineTransform2DFunction combineTransformFunc = [](const Transform2DComponent& transformA, const Transform2DComponent& transformB) {
            Transform2DComponent combinedTransform{};
            return Transform2DComponent{
                transformA.position + transformB.position,
                transformA.scale * transformB.scale,
                transformA.rotation * transformB.rotation,
                transformA.zIndex + transformB.zIndex,
            };
        };
        // Impl
        SceneNode sceneNode = sceneManager->GetCurrentScene()->GetSceneNode(entity);
        Transform2DComponent combinedTransform = Transform2DComponent{};
        Entity currentParent = sceneNode.parent;
        while (currentParent != NULL_ENTITY) {
            SceneNode nodeParent = sceneManager->GetCurrentScene()->GetSceneNode(currentParent);
            if (componentManager->HasComponent<Transform2DComponent>(nodeParent.entity)) {
                Transform2DComponent parentTransform = componentManager->GetComponent<Transform2DComponent>(nodeParent.entity);
                combinedTransform = combineTransformFunc(combinedTransform, parentTransform);
            }
            currentParent = nodeParent.parent;
        }
        // Combine Entity and parent transforms
        Transform2DComponent entityTransform = componentManager->GetComponent<Transform2DComponent>(entity);
        return combineTransformFunc(entityTransform, combinedTransform);
    }

    static Transform2DComponent TranslateCamera2D(const Transform2DComponent& transform2DComponent) {
        static CameraManager* cameraManager = CameraManager::GetInstance();
        Transform2DComponent translatedTransform{};
        Camera2D camera = cameraManager->GetCurrentCamera();
        translatedTransform.position = (transform2DComponent.position - camera.viewport + camera.offset) * camera.zoom;
        translatedTransform.scale = transform2DComponent.scale * camera.zoom;
        return translatedTransform;
    }

    static Transform2DComponent TranslateEntityTransform(Entity entity) {
        static ComponentManager* componentManager = ComponentManager::GetInstance();
        Transform2DComponent entityTransform = GetEntityCombinedParentsTransforms(entity);
        SceneComponent sceneComponent = componentManager->GetComponent<SceneComponent>(entity);
        if (!sceneComponent.ignoreCamera) {
            entityTransform = TranslateCamera2D(entityTransform);
        }
        return entityTransform;
    }
};
