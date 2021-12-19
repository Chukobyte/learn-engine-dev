#pragma once

#include "../ecs/ecs_orchestrator.h"
#include "../ecs/component/components/animated_sprite_component.h"

class AnimationUtils {
  public:
    static void SetAnimation(Entity entity, const std::string& animationName, bool setPlayingOnNewAnim = false) {
        static ECSOrchestrator* ecsOrchestrator = ECSOrchestrator::GetInstance();
        AnimatedSpriteComponent animatedSpriteComponent = ecsOrchestrator->GetComponent<AnimatedSpriteComponent>(entity);
        if (animatedSpriteComponent.animations.count(animationName) > 0 && animatedSpriteComponent.currentAnimation.name != animationName) {
            animatedSpriteComponent.currentAnimation = animatedSpriteComponent.animations[animationName];
            animatedSpriteComponent.isPlaying = setPlayingOnNewAnim;
            ecsOrchestrator->UpdateComponent(entity, animatedSpriteComponent);
        }
    }

    static void PlayAnimation(Entity entity, const std::string& animationName) {
        SetAnimation(entity, animationName, true);
    }

    static void StopAnimation(Entity entity, const std::string& animationName) {
        static ECSOrchestrator* ecsOrchestrator = ECSOrchestrator::GetInstance();
        AnimatedSpriteComponent animatedSpriteComponent = ecsOrchestrator->GetComponent<AnimatedSpriteComponent>(entity);
        animatedSpriteComponent.isPlaying = false;
        ecsOrchestrator->UpdateComponent(entity, animatedSpriteComponent);
    }
};