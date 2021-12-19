#pragma once

#include "../ec_system.h"

#include "../../../scene/scene_node_utils.h"
#include "../../component/components/transform2d_component.h"
#include "../../component/components/sprite_component.h"
#include "../../../rendering/renderer_2d.h"

class SpriteRenderingECSystem : public ECSystem {
  private:
    Renderer2D *renderer2D = nullptr;
    ComponentManager *componentManager = nullptr;

  public:
    SpriteRenderingECSystem() : renderer2D(Renderer2D::GetInstance()), componentManager(ComponentManager::GetInstance()) {}

    void Render() override {
        if (IsEnabled()) {
            for (Entity entity : entities) {
                Transform2DComponent translatedTransform = SceneNodeUtils::TranslateEntityTransformIntoWorld(entity);
                SpriteComponent spriteComponent = componentManager->GetComponent<SpriteComponent>(entity);
                Vector2 drawDestinationSize = Vector2(spriteComponent.drawSource.w * translatedTransform.scale.x, spriteComponent.drawSource.h * translatedTransform.scale.y);
                spriteComponent.drawDestination = Rect2(translatedTransform.position, drawDestinationSize);
                renderer2D->SubmitSpriteBatchItem(
                    spriteComponent.texture,
                    spriteComponent.drawSource,
                    spriteComponent.drawDestination,
                    translatedTransform.zIndex,
                    translatedTransform.rotation,
                    spriteComponent.modulate,
                    spriteComponent.flipX,
                    spriteComponent.flipY
                );
            }
        }
    }
};
