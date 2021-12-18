#pragma once

#include "../../../rendering/font.h"
#include "../../../rendering/color.h"

struct TextLabelComponent {
    std::string text;
    Font *font = nullptr;
    Color color;
};
