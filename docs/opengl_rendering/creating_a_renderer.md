# Creating a Renderer

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

## Renderer2D
