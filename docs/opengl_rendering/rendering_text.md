# Rendering Text

## Font

### Freetype Library

Simple Engine uses the [FreeType](https://freetype.org/) to help render font.

### Font Class

```c++
#ifndef FONT_H
#define FONT_H

#include <iostream>

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

#include "./re/math/math.h"

#include "../utils/file_helper.h"
#include "../utils/logger.h"

struct Character {
    GLuint textureID;
    Vector2 size;
    Vector2 bearing;
    unsigned int advance;
};

class Font {
  private:
    std::string filePath;
    int size;
    bool valid = false;

    void LoadFont(FT_Library freeTypeLibrary, const char* fileName, int size) {
        static Logger *logger = Logger::GetInstance();
        if(!FileHelper::DoesFileExist(fileName)) {
            logger->Error("Freetype failed at path: %s", fileName);
        }
        FT_Face face;
        if(FT_New_Face(freeTypeLibrary, fileName, 0, &face)) {
            logger->Error("Freetype failed to load font!");
        } else {
            // set size to load glyphs. width set to 0 to dynamically adjust
            FT_Set_Pixel_Sizes(face, 0, size);

            // disable byte-alignment restriction
            glPixelStorei(GL_UNPACK_ALIGNMENT, 1);

            // load first 128 characters of ASCII set
            for(unsigned char c = 0; c < 128; c++) {
                // load character glyph
                if(FT_Load_Char(face, c, FT_LOAD_RENDER)) {
                    logger->Error("Freetype Failed to load Glyph");
                    continue;
                }
                // generate texture
                unsigned int textTexture;
                glGenTextures(1, &textTexture);
                glBindTexture(GL_TEXTURE_2D, textTexture);
                glTexImage2D(
                        GL_TEXTURE_2D,
                        0,
                        GL_RED,
                        face->glyph->bitmap.width,
                        face->glyph->bitmap.rows,
                        0,
                        GL_RED,
                        GL_UNSIGNED_BYTE,
                        face->glyph->bitmap.buffer
                        );
                // set texture options
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
                glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, GL_LINEAR);
                // store character for later use
                Character character = {
                        .textureID = textTexture,
                        .size = Vector2(face->glyph->bitmap.width, face->glyph->bitmap.rows),
                        .bearing = Vector2(face->glyph->bitmap_left, face->glyph->bitmap_top),
                        .advance = static_cast<unsigned int>(face->glyph->advance.x)
                };
                characters.insert(std::pair<char, Character>(c, character));
            }
            glBindTexture(GL_TEXTURE_2D, 0);
            valid = true;
        }
        FT_Done_Face(face);
    }

    void ConfigureVertex() {
        // configure VAO & VBO texture quads
        glGenVertexArrays(1, &VAO);
        glGenBuffers(1, &VBO);
        glBindVertexArray(VAO);
        glBindBuffer(GL_ARRAY_BUFFER, VBO);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * 6 * 4, NULL, GL_DYNAMIC_DRAW);
        glEnableVertexAttribArray(0);
        glVertexAttribPointer(0, 4, GL_FLOAT, GL_FALSE, 4 * sizeof(GLfloat), nullptr);
        glBindBuffer(GL_ARRAY_BUFFER, 0);
        glBindVertexArray(0);
    }
  public:
    std::map<GLchar, Character> characters;
    GLuint VAO;
    GLuint VBO;

    Font(FT_Library freeTypeLibrary, const char* fileName, int size) : filePath(std::string(fileName)), size(size) {
        LoadFont(freeTypeLibrary, fileName, size);
        ConfigureVertex();
    }

    std::string GetFilePath() const {
        return filePath;
    }

    int GetSize() const {
        return size;
    }

    bool IsValid() const {
        return valid;
    }
};

#endif //FONT_H
```

## Update Rendering

### Renderer Batcher

Adding `FontBatchItem` struct.  Also adding vector for font draw batches.

```c++
struct FontBatchItem {
    Font *font = nullptr;
    std::string text;
    float x = 0.0f;
    float y = 0.0f;
    float scale = 1.0f;
    Color color = Color(1.0f, 1.0f, 1.0f, 1.0f);
};

struct ZIndexDrawBatch {
    std::vector<SpriteBatchItem> spriteDrawBatches;
    std::vector<FontBatchItem> fontDrawBatches;
};
```

Adding function to batch draw font.

```c++
void RendererBatcher::BatchDrawFont(FontBatchItem fontBatchItem, int zIndex) {
    if (drawBatches.find(zIndex) == drawBatches.end()) {
        drawBatches.emplace(zIndex, ZIndexDrawBatch{});
    }
    drawBatches[zIndex].fontDrawBatches.emplace_back(fontBatchItem);
}
```

### Renderer2D

Function to submit font draw batch item.

```c++
void Renderer2D::SubmitFontBatchItem(Font *font, const std::string &text, float x, float y, int zIndex,
                                     float scale, Color color) {
    FontBatchItem fontBatchItem = {
        font,
        text,
        x,
        y,
        scale,
        color
    };
    rendererBatcher.BatchDrawFont(fontBatchItem, zIndex);
}
```

Next is a function to draw font.

```c++
void Renderer2D::DrawFont(Font *font, const std::string &text, float x, float y, float scale, Color color) {}
```

### Render Context

```c++
FT_Library freeTypeLibrary;

void InitializeFont() {
    if(FT_Init_FreeType(&freeTypeLibrary)) {
        Logger::GetInstance()->Error("Could not initialize FreeType Library!");
    }
}
```

### Asset Manager

Be sure to add `RenderContext` to `AssetManager`.

```c++
AssetManager::AssetManager() : logger(Logger::GetInstance()), renderContext(RenderContext::GetInstance()) {}
```

Also add functions for fonts.

```c++
void AssetManager::LoadFont(const std::string &fontId, const std::string &fontPath, int size) {
    Font *font = new Font(fontId, renderContext->freeTypeLibrary, fontPath.c_str(), size);
    fonts.emplace(fontId, font);
}

Font* AssetManager::GetFont(const std::string &fontId) {
    assert(fonts.count(fontId) > 0 && "Font hasn't been loaded!");
    return fonts[fontId];
}

bool AssetManager::HasFont(const std::string &fontId) const {
    return fonts.count(fontId) > 0;
}
```

## Render Text

In `InitializeRendering` we are adding:

```c++
renderContext->InitializeFont();
```

We will also load a font into asset manager with:

```c++
assetManager->LoadFont("assets/fonts/verdana.ttf", "assets/fonts/verdana.ttf", 20);
```

Next to render text, we call this within our `Render` function:

```c++
static Font *textFont = assetManager->GetFont("assets/fonts/verdana.ttf");
static const std::string &text = "Hello World!";
static Vector2 fontPosition = Vector2(windowCenter.x - 35.0f, windowCenter.y - 20.0f);
static Color fontColor = Color(1.0f, 1.0f, 1.0f);
renderer2D.SubmitFontBatchItem(textFont, text, fontPosition.x, fontPosition.y, 0, 1.0f, fontColor);
```

Once we run the engine, the following will be rendered to the screen:

![Render Text Screenshot](https://github.com/Chukobyte/learn-engine-dev/blob/main/assets/images/screenshots/led_3_1.PNG?raw=true)

We have finally rendered text to the screen!  All source code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/3.opengl_rendering/3.1.rendering_text).  Now that we have a way to render sprites and text to the screen it's now time to start adding implementing audio to play music and sound effects!
