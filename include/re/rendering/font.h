#pragma once

#include <iostream>

#include <string>
#include <map>

#include <ft2build.h>
#include FT_FREETYPE_H

#include <glad/glad.h>

#include "re/math/redmath.h"
#include "../utils/file_helper.h"
#include "../utils/logger.h"

struct Character {
    GLuint textureID;
    Vector2 size;
    Vector2 bearing;
    unsigned int advance;
};

class Font {
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

  private:
    std::string filePath;
    int size;
    bool valid = false;

    void LoadFont(FT_Library freeTypeLibrary, const char* fileName, int size) {
        static Logger *logger = Logger::GetInstance();
        if(!FileHelper::DoesFileExist(fileName)) {
            logger->Error("Font doesn't exist at path: %s", fileName);
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
};
