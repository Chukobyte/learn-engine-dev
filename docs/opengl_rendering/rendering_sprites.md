# Rendering Sprites

## Math Lib

### GLM

[GLM](https://github.com/g-truc/glm) is a header only math library based on the OpenGL Shading Language (GLSL) specifications.  Most things related to math such as vectors, matrices, etc... will be handled by GLM.

### Rectangle

The `Rectangle` class will mostly be used for defining the bounds of a sprite's draw source and draw destination which we'll get to later.

```c++
#ifndef RECTANGLE_H
#define RECTANGLE_H

#include <iostream>

#include <glm/glm.hpp>

class Rectangle {
  public:
    float x;
    float y;
    float w;
    float h;

    Rectangle(): x(0.0f), y(0.0f), w(0.0f), h(0.0f) {}

    Rectangle(float x, float y, float w, float h): x(x), y(y), w(w), h(h) {}

    Rectangle(float x, float y, glm::vec2 size): x(x), y(y), w(size.x), h(size.y) {}

    Rectangle(glm::vec2 position, glm::vec2 size): x(position.x), y(position.y), w(size.x), h(size.y) {}

    bool operator==(const Rectangle &otherRect2) const {
        return this->x == otherRect2.x && this->y == otherRect2.y && this->w == otherRect2.w && this->h == otherRect2.h;
    }

    bool operator!=(const Rectangle &otherRect2) const {
        return !(*this == otherRect2);
    }

    friend std::ostream& operator<<(std::ostream& os, const Rectangle &r);
};

#endif //RECTANGLE_H
```

```c++
#include "rectangle.h"

std::ostream& operator<<(std::ostream& os, const Rectangle &r) {
    os << "(" << r.x << ", " << r.y << ", " << r.w << ", " << r.h << ")";
    return os;
}
```

### Custom Math Header

Simple Engine will use GLM, but we may want to use another math library or write our own in the future.  To make this easier, will we define [type aliases](https://en.cppreference.com/w/cpp/language/type_alias) for math related classes.

```c++
#ifndef MATH_H
#define MATH_H

#include "rectangle.h"

using Vector2 = glm::vec2;
using IVector2 = glm::ivec2;
using Vector3 = glm::vec3;
using Matrix4 = glm::mat4;
using Rect2 = Rectangle;

#endif //MATH_H
```

## Color

Not much to explain other than needing a concept of color within the engine.

```c++
#ifndef COLOR_H
#define COLOR_H

#include <glad/glad.h>

class Color {
  public:
    float r = 1.0f;
    float g = 1.0f;
    float b = 1.0f;
    float a = 1.0f;

    Color() {}

    Color(GLfloat red, GLfloat green, GLfloat blue) : r(red), g(green), b(blue), a(1.0f) {}

    Color(GLfloat red, GLfloat green, GLfloat blue, GLfloat alpha) : r(red), g(green), b(blue), a(alpha) {}

    Color operator*(float value) {
        return Color(this->r * value, this->g * value, this->b * value, this->a);
    }
};

#endif //COLOR_H
```

## Renderer Batcher

```c++
#ifndef RENDERER_BATCHER_H
#define RENDERER_BATCHER_H

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "./game_lib/rendering/color.h"
#include "./game_lib/rendering/texture.h"
#include "./game_lib/math/math.h"

struct SpriteBatchItem {
    Texture *texture2D = nullptr;
    Rect2 sourceRectangle;
    Rect2 destinationRectangle;
    float rotation = 0.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
    bool flipX = false;
    bool flipY = false;
};

struct ZIndexDrawBatch {
    std::vector<SpriteBatchItem> spriteDrawBatches;
};

using RenderFlushFunction = std::function<void(const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch)>;

class RendererBatcher {
  private:
    std::map<int, ZIndexDrawBatch> drawBatches;

  public:
    void BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex);

    void Flush(const RenderFlushFunction &renderFlushFunction);
};

#endif //RENDERER_BATCHER_H
```

```c++
#include "./renderer_batcher.h"

void RendererBatcher::BatchDrawSprite(SpriteBatchItem spriteBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatches.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].spriteDrawBatches.emplace_back(spriteBatchItem);
}

void RendererBatcher::Flush(const RenderFlushFunction &renderFlushFunction) {
    for (const auto &pair : drawBatches) {
        const int zIndex = pair.first;
        const ZIndexDrawBatch &zIndexDrawBatch = pair.second;
        renderFlushFunction(zIndex, zIndexDrawBatch);
    }
    drawBatches.clear();
}
```

Explanation coming soon...

## Renderer2D

```c++
#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "renderer_batcher.h"

class Renderer2D {
  private:
    RendererBatcher rendererBatcher;

    void DrawSprite(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, float rotation,
                    Color color, bool flipX, bool flipY);

  public:
    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);

    void FlushBatches();
};


#endif //RENDERER_2D_H
```

```c++
#include "renderer_2d.h"

void Renderer2D::SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle,
                                       int zIndex, float rotation,
                                       Color color, bool flipX, bool flipY) {
    SpriteBatchItem spriteBatchItem = {
        texture2D,
        sourceRectangle,
        destinationRectangle,
        rotation,
        color,
        flipX,
        flipY
    };
    rendererBatcher.BatchDrawSprite(spriteBatchItem, zIndex);
}

void Renderer2D::FlushBatches() {
    const RenderFlushFunction &renderFlushFunction = [this] (const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch) {
        // Render Sprites
        for (const SpriteBatchItem &spriteBatchItem : zIndexDrawBatch.spriteDrawBatches) {
            DrawSprite(spriteBatchItem.texture2D,
                       spriteBatchItem.sourceRectangle,
                       spriteBatchItem.destinationRectangle,
                       spriteBatchItem.rotation,
                       spriteBatchItem.color,
                       spriteBatchItem.flipX,
                       spriteBatchItem.flipY);
        }
    };
    rendererBatcher.Flush(renderFlushFunction);
}

void Renderer2D::DrawSprite(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, float rotation,
                            Color color, bool flipX, bool flipY) {

}
```

## Render Sprites
