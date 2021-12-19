#pragma once

#include "../math/math.h"

struct Camera2D {
    Vector2 viewport = Vector2(0.0f, 0.0f);
    Vector2 zoom = Vector2(1.0f, 1.0f);
    Vector2 offset = Vector2(0.0f, 0.0f);
    Rect2 boundary = Rect2(-10000000.0f, -10000000.0f, 10000000.0f, 10000000.0f);
};
