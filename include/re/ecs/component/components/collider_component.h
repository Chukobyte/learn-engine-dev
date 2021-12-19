#pragma once

#include <vector>

#include "../../../math/redmath.h"
#include "../../../rendering/color.h"

const Color DEFAULT_COLLIDER_COMPONENT_COLOR = Color::NormalizedColor(95, 205, 228, 190);

enum class CollisionGroup : unsigned int {
    NONE = 0,
    ONE = 2,
    TWO = 4,
    THREE = 8,
    FOUR = 16,
    FIVE = 32,
    SIX = 64,
    SEVEN = 128,
    EIGHT = 256,
    NINE = 512,
    TEN = 1024,
    ALL = ONE | TWO | THREE | FOUR | FIVE | SIX | SEVEN | EIGHT | NINE | TEN
};

struct ColliderComponent {
  public:
    Rect2 collider = Rect2();
    Color color = DEFAULT_COLLIDER_COMPONENT_COLOR;
    CollisionGroup group = CollisionGroup::ONE;
    std::vector<Entity> collisionExceptions;
};
