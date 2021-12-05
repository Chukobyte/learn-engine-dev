#ifndef SPRITE_RENDERING_EC_SYSTEM_H
#define SPRITE_RENDERING_EC_SYSTEM_H

#include "../ec_system.h"

#include "../../component/components/transform2d_component.h"
#include "../../component/components/sprite_component.h"
#include "../../../rendering/renderer_2d.h"

class SpriteRenderingECSystem : public ECSystem {
  private:
    Renderer2D *renderer2D = nullptr;
    ComponentManager *componentManager = nullptr;

  public:
    SpriteRenderingECSystem() : renderer2D(Renderer2D::GetInstance()), componentManager(ComponentManager::GetInstance()) {
        enabled = true;
    }

    void Render() {
        if (IsEnabled()) {
            for (Entity entity : entities) {
                Transform2DComponent transform2DComponent = componentManager->GetComponent<Transform2DComponent>(entity);
                SpriteComponent spriteComponent = componentManager->GetComponent<SpriteComponent>(entity);
                Vector2 drawDestinationSize = Vector2(spriteComponent.drawSource.w * transform2DComponent.scale.x, spriteComponent.drawSource.h * transform2DComponent.scale.y);
                spriteComponent.drawDestination = Rect2(transform2DComponent.position, drawDestinationSize);
                renderer2D->SubmitSpriteBatchItem(
                    spriteComponent.texture,
                    spriteComponent.drawSource,
                    spriteComponent.drawDestination,
                    transform2DComponent.zIndex,
                    transform2DComponent.rotation,
                    spriteComponent.modulate,
                    spriteComponent.flipX,
                    spriteComponent.flipY
                );
            }
        }
    }
};

#endif //SPRITE_RENDERING_EC_SYSTEM_H
