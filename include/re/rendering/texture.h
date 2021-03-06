#pragma once

#include <glad/glad.h>

#include "../utils/logger.h"

class Texture {
  public:
    Texture(const char* filePath, unsigned int wrapS = GL_CLAMP_TO_BORDER, unsigned int wrapT = GL_CLAMP_TO_BORDER, unsigned int filterMin = GL_NEAREST, unsigned int filterMag = GL_NEAREST);
    Texture(const char* filePath, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMag);
    Texture(unsigned int width, unsigned int height, unsigned int colorValue = 255); // colorValue default to white
    ~Texture();
    void Bind() const;
    std::string GetFilePath() const;
    int GetWidth() const;
    int GetHeight() const;
    unsigned int GetImageFormat() const;
    unsigned char* GetData() const;
    bool IsValid() const;

  private:
    Logger *logger = nullptr;
    std::string fileName;
    GLuint ID = 0;
    unsigned char* data = nullptr;
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    // format
    GLint internalFormat = GL_RGBA;
    GLint imageFormat = GL_RGBA;
    // configuration
    GLint wrapS = GL_CLAMP_TO_BORDER;
    GLint wrapT = GL_CLAMP_TO_BORDER;
    GLint filterMin = GL_NEAREST;
    GLint filterMag = GL_NEAREST;

    void Generate();
    unsigned int GetWrapFromString(const std::string &wrap) const;
    unsigned int GetFilterFromString(const std::string &filter) const;
};
