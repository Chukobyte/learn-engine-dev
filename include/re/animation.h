#pragma once

#include <unordered_map>

#include "rendering/texture.h"
#include "math/math.h"

struct AnimationFrame {
    Texture *texture = nullptr;
    Rect2 drawSource;
    int frame;
};

struct Animation {
    std::string name;
    int speed;
    std::unordered_map<unsigned int, AnimationFrame> animationFrames;
    unsigned int frames; // Caches number of frames to system doesn't have to count elements of map
};
