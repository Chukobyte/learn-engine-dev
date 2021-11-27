#ifndef FONT_RENDERER_H
#define FONT_RENDERER_H

#include <glad/glad.h>

#include "shader.h"
#include "color.h"
#include "font.h"
#include "../math/math.h"
#include "../utils/project_properties.h"

static const std::string &OPENGL_SHADER_SOURCE_VERTEX_FONT =
        "#version 330 core\n"
        "layout (location = 0) in vec4 vertex; // (pos, tex)\n"
        "\n"
        "out vec2 texCoords;\n"
        "\n"
        "uniform mat4 projection;\n"
        "\n"
        "void main() {\n"
        "    gl_Position = projection * vec4(vertex.xy, 0.0f, 1.0f);\n"
        "    texCoords = vertex.zw;\n"
        "}";

static const std::string &OPENGL_SHADER_SOURCE_FRAGMENT_FONT =
        "#version 330 core\n"
        "in vec2 texCoords;\n"
        "out vec4 color;\n"
        "\n"
        "uniform sampler2D textValue;\n"
        "uniform vec4 textColor;\n"
        "\n"
        "void main() {\n"
        "    vec4 sampled = vec4(1.0f, 1.0f, 1.0f, texture(textValue, texCoords).r);\n"
        "    color = textColor * sampled;\n"
        "}";

static const OpenGLShaderSourceCode OPENGL_SHADER_SOURCE_FONT = OpenGLShaderSourceCode{
    .vertex = OPENGL_SHADER_SOURCE_VERTEX_FONT,
    .fragment = OPENGL_SHADER_SOURCE_FRAGMENT_FONT
};

struct FontDrawBatch {
    Font *font = nullptr;
    std::string text = "";
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
};

class FontRenderer {
  private:
    Shader shader;
    ProjectProperties *projectProperties = nullptr;

    static float ConvertMinMax(float input, float inputLow, float inputHigh, float outputLow, float outputHigh) {
        return (((input - inputLow) / (inputHigh - inputLow)) * (outputHigh - outputLow) + outputLow);
    }
  public:
    FontRenderer();

    void Draw(Font *font, const std::string &text, float x, float y, float scale = 1.0f, const Color color = Color(1.0f, 1.0f, 1.0f, 1.0f));

    void UpdateProjection();
};


#endif //FONT_RENDERER_H
