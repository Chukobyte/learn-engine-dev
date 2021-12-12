#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>

#include "shader.h"
#include "texture.h"
#include "color.h"
#include "../math/math.h"

#include "re/project_properties.h"

static const std::string &OPENGL_SHADER_SOURCE_VERTEX_SPRITE =
    "#version 330 core\n"
    "\n"
    "layout (location = 0) in vec4 vertex;\n"
    "\n"
    "out vec2 texCoord;\n"
    "\n"
    "uniform mat4 projection;\n"
    "uniform mat4 model;\n"
    "\n"
    "void main() {\n"
    "    texCoord = vertex.zw;\n"
    "    gl_Position = projection * model * vec4(vertex.xy, 0.0f, 1.0f);\n"
    "}\n"
    "";

static const std::string &OPENGL_SHADER_SOURCE_FRAGMENT_SPRITE =
    "#version 330 core\n"
    "\n"
    "in vec2 texCoord;\n"
    "out vec4 color;\n"
    "\n"
    "uniform sampler2D sprite;\n"
    "uniform vec4 spriteColor;\n"
    "\n"
    "void main() {\n"
    "    color = spriteColor * texture(sprite, texCoord);\n"
    "}\n"
    "";

static const OpenGLShaderSourceCode OPENGL_SHADER_SOURCE_SPRITE = OpenGLShaderSourceCode{
    .vertex = OPENGL_SHADER_SOURCE_VERTEX_SPRITE,
    .fragment = OPENGL_SHADER_SOURCE_FRAGMENT_SPRITE
};

class SpriteRenderer {
  private:
    Shader shader;
    GLuint quadVAO;
    GLuint quadVBO;
    ProjectProperties *projectProperties = nullptr;

  public:
    SpriteRenderer();

    void Draw(Texture *texture2D, const Rect2 &sourceRectangle, const Rect2 &destinationRectangle, float rotation,
              const Color &color, bool flipX, bool flipY);
};


#endif //SPRITE_RENDERER_H
