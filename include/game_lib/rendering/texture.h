#ifndef TEXTURE_H
#define TEXTURE_H

#include <glad/glad.h>

#include "../utils/logger.h"

class Texture {
  private:
    Logger *logger = nullptr;
    std::string fileName;

    GLuint ID = 0;
    unsigned char* data = nullptr;
    int width = 0;
    int height = 0;
    int nrChannels = 0;
    // format
    unsigned int internalFormat = GL_RGBA;
    unsigned int imageFormat = GL_RGBA;
    // configuration
    unsigned int wrapS = GL_CLAMP_TO_BORDER;
    unsigned int wrapT = GL_CLAMP_TO_BORDER;
    unsigned int filterMin = GL_NEAREST;
    unsigned int filterMax = GL_NEAREST;

    void Initialize(const char* filePath);

    void Generate();

    unsigned int GetWrapFromString(const std::string &wrap) const;

    unsigned int GetFilterFromString(const std::string &filter) const;
  public:
    Texture(const char* filePath);

    Texture(const char* filePath, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMax);

    Texture(const char* filePath, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMax);

    ~Texture();

    void Bind() const;

    std::string GetFilePath() const;

    int GetWidth() const;

    int GetHeight() const;

    unsigned int GetImageFormat() const;

    unsigned char* GetData() const;

    bool IsValid() const;
};


#endif //TEXTURE_H
