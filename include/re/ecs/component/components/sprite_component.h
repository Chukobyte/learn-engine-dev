#pragma once

#include "../component.h"
#include "../../../rendering/texture.h"
#include "../../../rendering/color.h"
#include "re/math/redmath.h"

struct SpriteComponent {
    Texture* texture = nullptr;
    Rect2 drawSource;
    bool flipX = false;
    bool flipY = false;
    Color modulate = Color(1.0f, 1.0f, 1.0f, 1.0f);
    Rect2 drawDestination; // Updated when drawing to position
};
