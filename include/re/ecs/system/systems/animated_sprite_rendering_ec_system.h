#pragma once

#include "../ec_system.h"

#include "../../component/components/transform2d_component.h"
#include "../../component/components/animated_sprite_component.h"
#include "../../../rendering/renderer_2d.h"

class AnimatedSpriteRenderingECSystem : public ECSystem {
  private:
    Renderer2D *renderer2D = nullptr;
    ComponentManager *componentManager = nullptr;

  public:
    AnimatedSpriteRenderingECSystem() : renderer2D(Renderer2D::GetInstance()), componentManager(ComponentManager::GetInstance()) {}

    void Render() override {
        if (IsEnabled()) {
            for (Entity entity : entities) {
                Transform2DComponent transform2DComponent = componentManager->GetComponent<Transform2DComponent>(entity);
                AnimatedSpriteComponent animatedSpriteComponent = componentManager->GetComponent<AnimatedSpriteComponent>(entity);
                // Process Animation
                Animation& currentAnimation = animatedSpriteComponent.currentAnimation;
                AnimationFrame currentFrame = currentAnimation.animationFrames[animatedSpriteComponent.currentFrameIndex];
                if (animatedSpriteComponent.isPlaying) {
                    unsigned int newIndex = static_cast<unsigned int>((SDL_GetTicks() / currentAnimation.speed) % currentAnimation.frames);
                    if (newIndex != animatedSpriteComponent.currentFrameIndex) {
                        // Index changed
                        currentFrame = currentAnimation.animationFrames[newIndex];
                        if (newIndex + 1 == currentAnimation.frames) {
                            // Animation Finished
                        }
                        animatedSpriteComponent.currentFrameIndex = newIndex;
                        componentManager->UpdateComponent(entity, animatedSpriteComponent);
                    }
                }
                // Submit draw batch
                Vector2 drawDestinationSize = Vector2(currentFrame.drawSource.w * transform2DComponent.scale.x, currentFrame.drawSource.h * transform2DComponent.scale.y);
                Rect2 drawDestination = Rect2(transform2DComponent.position, drawDestinationSize);
                renderer2D->SubmitSpriteBatchItem(
                    currentFrame.texture,
                    currentFrame.drawSource,
                    drawDestination,
                    transform2DComponent.zIndex,
                    transform2DComponent.rotation,
                    animatedSpriteComponent.modulate,
                    animatedSpriteComponent.flipX,
                    animatedSpriteComponent.flipY
                );
            }
        }
    }
};
