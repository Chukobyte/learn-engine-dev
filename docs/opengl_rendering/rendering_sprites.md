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

## 2D Rendering

### Shaders



### SpriteRenderer

```c++
#ifndef SPRITE_RENDERER_H
#define SPRITE_RENDERER_H

#include <glad/glad.h>

#include "./game_lib/rendering/shader.h"
#include "./game_lib/rendering/texture.h"
#include "./game_lib/rendering/color.h"
#include "./game_lib/math/math.h"

#include "project_properties.h"
#include "../../../../include/game_lib/math/math.h"

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
    static Rect2 drawDestinationRect = Rect2(
                                           projectProperties->GetWindowWidth() / 2,
                                           projectProperties->GetWindowHeight() / 2,
                                           32,
                                           32);
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

The source code for this section can be found here.  Now that we have rendered a sprite it's now time to render font.
