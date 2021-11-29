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

***Red Engine*** will use GLM, but we may want to use another math library or write our own in the future.  To make this easier, will we define [type aliases](https://en.cppreference.com/w/cpp/language/type_alias) for math related classes.

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

The `RendererBatcher` class will be used to take draw calls and batch them to be used during rendering.  Typically, batching is done to limit draw calls for efficiency and performance reasons but for simplicity’s sake this batcher will be designed to just defer and submit draw calls during the `Render` phase.

```c++
#ifndef RENDERER_BATCHER_H
#define RENDERER_BATCHER_H

#include <vector>
#include <map>
#include <string>
#include <functional>

#include "./re/rendering/color.h"
#include "./re/rendering/texture.h"
#include "./re/math/math.h"

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

The first thing to point out is the `SpriteBatchItem` struct as this will be used to determine what to render for a sprite.  ***Red Engine*** will group draw batches by their z indices.  The `ZIndexDrawBatch` struct will represent a draw batch for a particular z index.  We also define an alias `RenderFlushFunction` to be used as a lambda function to render flushed draw batches by the Renderer which we'll get to later.

`RendererBatcher` class is currently simple as it has one map and two functions.  The `drawBatches` map maintains the order of draw batches based on their z indices.  `BatchDrawSprite` will submit a batch item to the batcher to draw during the render phase.  The `Flush` function is called during the render phase to render batch items submitted and will clear them when completed.

## 2D Rendering

### Shaders

```c++
#ifndef SHADER_H
#define SHADER_H

#include <string>
#include <fstream>
#include <sstream>
#include <iostream>

#include <glad/glad.h>

#include "./re/math/math.h"
#include "./color.h"
#include "../utils/logger.h"

struct OpenGLShaderSourceCode {
    std::string vertex;
    std::string fragment;
};

class Shader {
  private:
    unsigned int ID;
    Logger *logger = nullptr;

    void CheckCompileErrors(unsigned int shader, const std::string &type);

  public:
    Shader();

    Shader(const std::string &vertexPath, const std::string &fragmentPath);

    Shader(OpenGLShaderSourceCode openGlShaderSourceCode);

    ~Shader();

    OpenGLShaderSourceCode GetOpenGLShaderSourceFromPaths(const std::string &vertexPath, const std::string &fragmentPath);

    void Compile(OpenGLShaderSourceCode openGlShaderSourceCode);

    void Use();

    void SetBool(const std::string &name, bool value) const;

    void SetInt(const std::string &name, int value) const;

    void SetFloat(const std::string &name, float value) const;

    void SetVec2Float(const std::string &name, float v1, float v2) const;

    void SetVec2Float(const std::string &name, const Vector2 &value) const;

    void SetVec3Float(const std::string &name, const Color &value) const;

    void SetVec3Float(const std::string &name, const Vector3 &value) const;

    void SetVec3Float(const std::string &name, float v1, float v2, float v3) const;

    void SetVec4Float(const std::string &name, float v1, float v2, float v3, float v4) const;

    void SetVec4Float(const std::string &name, const Color &value) const;

    void SetMatrix4Float(const std::string &name, const Matrix4 &mat) const;
};

#endif //SHADER_H
```

```c++
#include "shader.h"

#include <glm/gtc/type_ptr.hpp>

static const std::string VERTEX = "VERTEX";
static const std::string FRAGMENT = "FRAGMENT";
static const std::string PROGRAM = "PROGRAM";

Shader::Shader() : logger(Logger::GetInstance()) {}

Shader::Shader(const std::string &vertexPath, const std::string &fragmentPath) : logger(Logger::GetInstance()) {
    Compile(GetOpenGLShaderSourceFromPaths(vertexPath, fragmentPath));
}

Shader::Shader(OpenGLShaderSourceCode openGlShaderSourceCode) : logger(Logger::GetInstance()) {
    Compile(openGlShaderSourceCode);
}

Shader::~Shader() {}

OpenGLShaderSourceCode Shader::GetOpenGLShaderSourceFromPaths(const std::string &vertexPath, const std::string &fragmentPath) {
    std::ifstream vertexShaderFile;
    std::ifstream fragmentShaderFile;
    vertexShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    fragmentShaderFile.exceptions(std::ifstream::failbit | std::ifstream::badbit);
    OpenGLShaderSourceCode openGlShaderSourceCode;
    try {
        // TODO: Validate file exists...
        vertexShaderFile.open(vertexPath.c_str());
        fragmentShaderFile.open(fragmentPath.c_str());
        std::stringstream vertexShaderStream, fragmentShaderStream;
        vertexShaderStream << vertexShaderFile.rdbuf();
        fragmentShaderStream << fragmentShaderFile.rdbuf();
        vertexShaderFile.close();
        fragmentShaderFile.close();
        openGlShaderSourceCode.vertex = vertexShaderStream.str();
        openGlShaderSourceCode.fragment = fragmentShaderStream.str();
    } catch(std::ifstream::failure& e) {
        logger->Error("Error reading shader files!\n"
                      "vertex path = '%s'\nfragment path = '%s'!", vertexPath.c_str(), fragmentPath.c_str());
    }
    return openGlShaderSourceCode;
}

void Shader::Compile(OpenGLShaderSourceCode openGlShaderSourceCode) {
    unsigned int vertex, fragment;
    const char *vertexSource = openGlShaderSourceCode.vertex.c_str();
    const char *fragmentSource = openGlShaderSourceCode.fragment.c_str();
    // vertex
    vertex = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertex, 1, &vertexSource, nullptr);
    glCompileShader(vertex);
    CheckCompileErrors(vertex, VERTEX);
    // fragment
    fragment = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragment, 1, &fragmentSource, nullptr);
    glCompileShader(fragment);
    CheckCompileErrors(fragment, FRAGMENT);
    // shader
    ID = glCreateProgram();
    glAttachShader(ID, vertex);
    glAttachShader(ID, fragment);
    glLinkProgram(ID);
    CheckCompileErrors(ID, PROGRAM);
    glDeleteShader(vertex);
    glDeleteShader(fragment);
}

void Shader::Use() {
    glUseProgram(ID);
}

void Shader::SetBool(const std::string &name, bool value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), (int)value);
}

void Shader::SetInt(const std::string &name, int value) const {
    glUniform1i(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetFloat(const std::string &name, float value) const {
    glUniform1f(glGetUniformLocation(ID, name.c_str()), value);
}

void Shader::SetVec2Float(const std::string &name, float v1, float v2) const {
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), v1, v2);
}

void Shader::SetVec2Float(const std::string &name, const Vector2 &value) const {
    glUniform2f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y);
}

void Shader::SetVec3Float(const std::string &name, float v1, float v2, float v3) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3);
}

void Shader::SetVec3Float(const std::string &name, const Color &value) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.r, value.g, value.b);
}

void Shader::SetVec3Float(const std::string &name, const Vector3 &value) const {
    glUniform3f(glGetUniformLocation(this->ID, name.c_str()), value.x, value.y, value.z);
}

void Shader::SetVec4Float(const std::string &name, float v1, float v2, float v3, float v4) const {
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), v1, v2, v3, v4);
}

void Shader::SetVec4Float(const std::string &name, const Color &value) const {
    glUniform4f(glGetUniformLocation(this->ID, name.c_str()), value.r, value.g, value.b, value.a);
}

void Shader::SetMatrix4Float(const std::string &name, const Matrix4 &mat) const {
    glUniformMatrix4fv(glGetUniformLocation(this->ID, name.c_str()), 1, GL_FALSE, glm::value_ptr(mat));
}

void Shader::CheckCompileErrors(unsigned int shader, const std::string &type) {
    int success;
    char infoLog[1024];
    if(type == PROGRAM) {
        glGetProgramiv(shader, GL_LINK_STATUS, &success);
        if(!success) {
            glGetProgramInfoLog(shader, 1024, nullptr, infoLog);
            logger->Error("Shader type '%s' linking failed!\n%s", type.c_str(), infoLog);
        }
    } else {
        glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
        if(!success) {
            glGetShaderInfoLog(shader, 1024, nullptr, infoLog);
            logger->Error("Shader type '%s' compilation failed!\n%s", type.c_str(), infoLog);
        }
    }
}
```

Explanation coming soon...

### SpriteRenderer

```c++
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>

#include "./re/rendering/shader.h"
#include "./re/rendering/texture.h"
#include "./re/rendering/color.h"
#include "./re/math/math.h"

#include "project_properties.h"

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
```

```c++
#include "sprite_renderer.h"

#include <glm/gtc/matrix_transform.hpp>

SpriteRenderer::SpriteRenderer() : projectProperties(ProjectProperties::GetInstance()) {
    GLfloat vertices[] = {
        // positions // texture coordinates
        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f,
        0.0f, 0.0f, 0.0f, 0.0f,

        0.0f, 1.0f, 0.0f, 1.0f,
        1.0f, 1.0f, 1.0f, 1.0f,
        1.0f, 0.0f, 1.0f, 0.0f
    };

    glGenVertexArrays(1, &quadVAO);
    glGenBuffers(1, &quadVBO);

    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

    glBindVertexArray(quadVAO);
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), (void*) nullptr);
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glBindVertexArray(0);

    shader = Shader(OPENGL_SHADER_SOURCE_SPRITE);
    shader.Use();
    shader.SetInt("sprite", 0);
    Matrix4 projection = glm::ortho(0.0f, static_cast<float>(projectProperties->GetWindowWidth()), static_cast<float>(projectProperties->GetWindowHeight()), 0.0f, -1.0f, 1.0f);
    shader.SetMatrix4Float("projection", projection);
}

void
SpriteRenderer::Draw(Texture *texture2D, const Rect2 &sourceRectangle, const Rect2 &destinationRectangle, float rotation, const Color &color,
                     bool flipX, bool flipY) {
    // 1. Translation
    Matrix4 model = glm::mat4(1.0f);
    model = glm::translate(model, glm::vec3(destinationRectangle.x, destinationRectangle.y, 0.0f));  // first translate (transformations are: scale happens first, then rotation, and then final translation happens; reversed order)
    // 2. Rotation
    model = glm::translate(model, Vector3(0.5f * destinationRectangle.w, 0.5f * destinationRectangle.h, 0.0f)); // move origin of rotation to center of quad
    model = glm::rotate(model, glm::radians(rotation), Vector3(0.0f, 0.0f, 1.0f)); // then rotate
    model = glm::translate(model, Vector3(-0.5f * destinationRectangle.w, -0.5f * destinationRectangle.h, 0.0f)); // move origin back
    // 3. Scaling
    model = glm::scale(model, Vector3(destinationRectangle.w, destinationRectangle.h, 1.0f)); // last scale

    glBindVertexArray(quadVAO);
    glBindBuffer(GL_ARRAY_BUFFER, quadVBO);

    shader.Use();
    shader.SetMatrix4Float("model", model);
    shader.SetVec4Float("spriteColor", color.r, color.g, color.b, color.a);

    glActiveTexture(GL_TEXTURE0);
    texture2D->Bind();

    // render subimage based on source rectangle
    glPixelStorei(GL_UNPACK_ROW_LENGTH, texture2D->GetWidth());
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, sourceRectangle.x);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, sourceRectangle.y);

    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGBA, sourceRectangle.w, sourceRectangle.h, 0, texture2D->GetImageFormat(), GL_UNSIGNED_BYTE, texture2D->GetData());

    glPixelStorei(GL_UNPACK_ROW_LENGTH, 0);
    glPixelStorei(GL_UNPACK_SKIP_PIXELS, 0);
    glPixelStorei(GL_UNPACK_SKIP_ROWS, 0);

    // Render Container
    GLfloat vertices[6][4] = {
            // positions // texture coordinates
            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f,
            0.0f, 0.0f, 0.0f, 0.0f,

            0.0f, 1.0f, 0.0f, 1.0f,
            1.0f, 1.0f, 1.0f, 1.0f,
            1.0f, 0.0f, 1.0f, 0.0f
    };

    static const int VERT_X = 2;
    static const int VERT_Y = 3;
    if (flipX) {
        vertices[0][VERT_X] = 1.0f;
        vertices[1][VERT_X] = 0.0f;
        vertices[2][VERT_X] = 1.0f;
        vertices[3][VERT_X] = 1.0f;
        vertices[4][VERT_X] = 0.0f;
        vertices[5][VERT_X] = 0.0f;
    }

    if (flipY) {
        vertices[0][VERT_Y] = 0.0f;
        vertices[1][VERT_Y] = 1.0f;
        vertices[2][VERT_Y] = 1.0f;
        vertices[3][VERT_Y] = 0.0f;
        vertices[4][VERT_Y] = 0.0f;
        vertices[5][VERT_Y] = 1.0f;
    }

    glBufferSubData(GL_ARRAY_BUFFER, 0, sizeof(vertices), vertices);
    glDrawArrays(GL_TRIANGLES, 0, 6);

    glBindVertexArray(0);
}
```

### Renderer2D

```c++
#ifndef RENDERER_2D_H
#define RENDERER_2D_H

#include "renderer_batcher.h"
#include "sprite_renderer.h"

class Renderer2D {
  private:
    RendererBatcher rendererBatcher;
    SpriteRenderer *spriteRenderer = nullptr;

  public:
    Renderer2D() = default;

    ~Renderer2D();

    void Initialize();

    void SubmitSpriteBatchItem(Texture *texture2D, Rect2 sourceRectangle, Rect2 destinationRectangle, int zIndex, float rotation = 0.0f, Color color = Color(1.0f, 1.0f, 1.0f), bool flipX = false, bool flipY = false);

    void FlushBatches();
};


#endif //RENDERER_2D_H
```

```c++
#include "renderer_2d.h"

#include <cassert>
#include <glad/glad.h>

Renderer2D::~Renderer2D() {
    if (spriteRenderer != nullptr) {
        delete spriteRenderer;
    }
}

void Renderer2D::Initialize() {
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    spriteRenderer = new SpriteRenderer();
}


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
    assert(spriteRenderer != nullptr && "SpriteRenderer is NULL, initialize the Renderer2D before using!");

    const RenderFlushFunction &renderFlushFunction = [this] (const int zIndex, const ZIndexDrawBatch &zIndexDrawBatch) {
        for (const SpriteBatchItem &spriteBatchItem : zIndexDrawBatch.spriteDrawBatches) {
            spriteRenderer->Draw(spriteBatchItem.texture2D,
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
```

## Render Sprites

Updating constructor to initialize member pointer variables.

```c++
GameEngine::GameEngine() :
    projectProperties(ProjectProperties::GetInstance()),
    engineContext(GameEngineContext::GetInstance()),
    renderContext(RenderContext::GetInstance()),
    assetManager(AssetManager::GetInstance()),
    fpsCounter(FPSCounter::GetInstance()),
    logger(Logger::GetInstance()) {
    Initialize();
}
```

Implementing the private `InitializeRendering` function.

```c++
void GameEngine::InitializeRendering() {
    // OpenGL attributes
    SDL_GL_SetAttribute(SDL_GL_DOUBLEBUFFER, 1);
    SDL_GL_SetAttribute(SDL_GL_DEPTH_SIZE, 24);
    SDL_GL_SetAttribute(SDL_GL_STENCIL_SIZE, 8);

    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
    SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

    renderContext->window = SDL_CreateWindow(
                                projectProperties->GetGameTitle().c_str(),
                                SDL_WINDOWPOS_CENTERED,
                                SDL_WINDOWPOS_CENTERED,
                                projectProperties->GetWindowWidth(),
                                projectProperties->GetWindowHeight(),
                                renderContext->windowFlags);
    renderContext->glContext = SDL_GL_CreateContext(renderContext->window);
    renderContext->currentWindowWidth = projectProperties->GetWindowWidth();
    renderContext->currentWindowHeight = projectProperties->GetWindowHeight();

    if (!gladLoadGLLoader((GLADloadproc)SDL_GL_GetProcAddress)) {
        logger->Error("Couldn't initialize glad");
    }

    renderer2D.Initialize();

    // Temp Load Assets
    assetManager->LoadTexture("assets/images/melissa_walk_animation.png", "assets/images/melissa_walk_animation.png");
}
```

Now we will implement the `Render` function.

```c++
void GameEngine::Render() {
    glClearColor(projectProperties->backgroundClearColor.r,
                 projectProperties->backgroundClearColor.g,
                 projectProperties->backgroundClearColor.b,
                 projectProperties->backgroundClearColor.a);
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT | GL_STENCIL_BUFFER_BIT);

    // Render Sprites
    static Texture *mellisaWalkTexture = assetManager->GetTexture("assets/images/melissa_walk_animation.png");
    static Rect2 drawSourceRect = Rect2(0, 0, 32, 32);
    static Rect2 drawDestinationRect = Rect2(windowCenter.x, windowCenter.y, drawSourceRect.w, drawSourceRect.h);
    renderer2D.SubmitSpriteBatchItem(mellisaWalkTexture, drawSourceRect, drawDestinationRect, 0);

    // Flush
    renderer2D.FlushBatches();


    SDL_GL_SwapWindow(renderContext->window);
}
```

Lastly we are going to process input by checking if an `SDL_QUIT` [event type](https://wiki.libsdl.org/SDL_EventType) was triggered.  If we click the 'X' in the top right part of the window, we will now close the window and shutdown the engine.

```c++
void GameEngine::ProcessInput() {
    SDL_Event event;
    while (SDL_PollEvent(&event)) {
        switch(event.type) {
        case SDL_QUIT:
            engineContext->SetRunning(false);
            break;
        case SDL_WINDOWEVENT:
            switch (event.window.event) {
            case SDL_WINDOWEVENT_RESIZED:
                renderContext->currentWindowWidth = event.window.data1;
                renderContext->currentWindowHeight = event.window.data2;
                glViewport(0, 0, renderContext->currentWindowWidth, renderContext->currentWindowHeight);
                break;
            }
            break;
        }
    }
}
```

After executing the code, this will be rendered to the screen:

![Rendering Sprite Screenshot](https://github.com/Chukobyte/learn-engine-dev/blob/main/assets/images/screenshots/led_3_0.PNG?raw=true)

The source code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/3.opengl_rendering/3.0.rendering_sprites).  Now that we have rendered a sprite it's now time to render font.
