#ifndef TRANSFORM2D_COMPONENT_H
#define TRANSFORM2D_COMPONENT_H

#include "../../../math/math.h"

struct Transform2DComponent {
    Vector2 position = Vector2(0.0f, 0.0f);
    Vector2 scale = Vector2(1.0f, 1.0f);
    float rotation = 0.0f;
    int zIndex = 0;
    bool isZIndexRelativeToParent = true;
    bool ignoreCamera = false;
};

#endif //TRANSFORM2D_COMPONENT_H
