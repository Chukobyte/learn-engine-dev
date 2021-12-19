#pragma once

#include "redmath.h"

namespace RedMath {
static constexpr float FLOAT_EPSILON = 0.001f;

static bool IsFloatGreaterOrEqual(float a, float b) {
    return fabs(a - b) < FLOAT_EPSILON || a > b;
}

namespace Collision {
static bool AABB(Rect2 source, Rect2 target) {
    return IsFloatGreaterOrEqual(source.x + source.w, target.x) &&
           IsFloatGreaterOrEqual(target.x + target.w, source.x) &&
           IsFloatGreaterOrEqual(source.y + source.h, target.y) &&
           IsFloatGreaterOrEqual(target.y + target.h, source.y);
}
}
}
