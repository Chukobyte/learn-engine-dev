#pragma once

#include <string>
#include <unordered_map>

#include "re/animation/animation.h"

struct AnimatedSpriteComponent {
    std::unordered_map<std::string, Animation> animations;
    Animation currentAnimation; // Preselects first added animation
    bool isPlaying;
    bool flipX = false;
    bool flipY = false;
    Color modulate = Color(1.0f, 1.0f, 1.0f, 1.0f);
    unsigned int currentFrameIndex = 0;
};
