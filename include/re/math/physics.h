#pragma once

#include "redmath.h"

namespace RedMath {
static constexpr float FLOAT_EPSILON = 0.001f;

static bool IsFloatGreaterOrEqual(float a, float b) {
    return fabs(a - b) < FLOAT_EPSILON || a > b;
}

namespace Collision {
static bool AABB(Rect2 source, Rect2 target) {
    return IsFloatGreaterOrEqual(sourceRect.x + sourceRect.w, targetRect.x) &&
           IsFloatGreaterOrEqual(targetRect.x + targetRect.w, sourceRect.x) &&
           IsFloatGreaterOrEqual(sourceRect.y + sourceRect.h, targetRect.y) &&
           IsFloatGreaterOrEqual(targetRect.y + targetRect.h, sourceRect.y);
}
}
}
