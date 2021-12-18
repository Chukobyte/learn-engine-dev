#pragma once

#include <string>
#include <unordered_map>

#include "../../../animation.h"

struct AnimatedSpriteComponent {
    std::unordered_map<std::string, Animation> animations;
    Animation currentAnimation; // Preselects first added animation
    unsigned int currentFrameIndex;
    bool isPlaying;
    bool flipX = false;
    bool flipY = false;
    Color modulate = Color(1.0f, 1.0f, 1.0f, 1.0f);
};
