#pragma once

#include <vector>

#include "../../../math/redmath.h"
#include "../../../rendering/color.h"

const Color DEFAULT_COLLIDER_COMPONENT_COLOR = Color::NormalizedColor(95, 205, 228, 190);

struct ColliderComponent {
  public:
    Rect2 collider = Rect2();
    Color color = DEFAULT_COLLIDER_COMPONENT_COLOR;
    std::vector<Entity> collisionExceptions;
};
