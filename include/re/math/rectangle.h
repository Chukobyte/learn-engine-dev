#pragma once

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
