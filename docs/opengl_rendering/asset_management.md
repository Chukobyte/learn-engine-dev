# Asset Management

## Textures

### Glad

As you already know, we will be using the OpenGL graphics API for our renderer.  [Glad](https://github.com/Dav1dde/glad), which is an OpenGL loading library, will be used to by ***Red Engine*** to access OpenGL functions.  We will modify the build of the engine by adding the generated `glad.c` file to Makefile.

```makefile
SRC_C = $(wildcard $(INCLUDE_DIR)/glad/glad.c)
```

### stb_image

[stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) is an image loading library which is part of a larger library named [stb](https://github.com/nothings/stb).  All functions from the [stb_image](https://github.com/nothings/stb/blob/master/stb_image.h) library will have the prefix `stbi_`.

### Texture Class

Instances of the `Texture` class will be used to render sprites to the screen.

```c++
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
    unsigned int filterMag = GL_NEAREST;

    void Initialize(const char* fileName);

    void Initialize(void *buffer, size_t bufferSize);

    void Generate();

    unsigned int GetWrapFromString(const std::string &wrap) const;

    unsigned int GetFilterFromString(const std::string &filter) const;
  public:
    Texture(const char* fileName);

    Texture(const char* fileName, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMag);

    Texture(const char* fileName, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMag);

    Texture(void *buffer, size_t bufferSize);

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
```

```c++
#include "texture.h"

#include <stb_image/stb_image.h>

Texture::Texture(const char* fileName) {
    Initialize(fileName);
}

Texture::Texture(const char* fileName, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMag) :
    wrapS(wrapS), wrapT(wrapT), filterMin(filterMin), filterMag(filterMag) {
    Initialize(fileName);
}

Texture::Texture(const char* fileName, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMag) :
    wrapS(GetWrapFromString(wrapS)), wrapT(GetWrapFromString(wrapT)), filterMin(GetFilterFromString(filterMin)), filterMag(GetFilterFromString(filterMag)) {
    Initialize(fileName);
}

Texture::Texture(void *buffer, size_t bufferSize) {
    Initialize(buffer, bufferSize);
}

Texture::~Texture() {
    stbi_image_free(this->data);
    this->data = nullptr;
}

void Texture::Initialize(const char* fileName) {
    this->logger = Logger::GetInstance();
    this->fileName = std::string(fileName);
    // load image, create texture, and generate mipmaps
    stbi_set_flip_vertically_on_load(false);
    this->data = stbi_load(fileName, &width, &height, &nrChannels, 0);
    if(IsValid()) {
        Generate();
    } else {
        logger->Error("Texture failed to load at: %s", fileName);
    }
}

void Texture::Initialize(void *buffer, size_t bufferSize) {
    // load image, create texture, and generate mipmaps
    stbi_set_flip_vertically_on_load(false);
    this->data = stbi_load_from_memory((unsigned char*) buffer, bufferSize, &width, &height, &nrChannels, 0);
    if(IsValid()) {
        Generate();
    } else {
        logger->Error("Texture failed to load at texture from memory!");
    }
}

void Texture::Generate() {
    if (nrChannels == 1) {
        imageFormat = GL_RED;
    } else if (nrChannels == 3) {
        imageFormat = GL_RGB;
    } else if (nrChannels == 4) {
        imageFormat = GL_RGBA;
    }

    // Create texture
    glGenTextures(1, &this->ID);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, this->internalFormat, this->width, this->height, 0, this->imageFormat, GL_UNSIGNED_BYTE, this->data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, this->wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, this->wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, this->filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, this->filterMag);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetWrapFromString(const std::string &wrap) const {
    if (wrap == "clamp_to_border") {
        return GL_CLAMP_TO_BORDER;
    } else if (wrap == "repeat") {
        return GL_REPEAT;
    }
    logger->Error("'%s' is an invalid wrap value!", wrap.c_str());
    return wrapS;
}

unsigned int Texture::GetFilterFromString(const std::string &filter) const {
    if (filter == "nearest") {
        return GL_NEAREST;
    } else if (filter == "linear") {
        return GL_LINEAR;
    }
    logger->Error("'%s' is an invalid filter value!", filter.c_str());
    return filterMin;
}

void Texture::Bind() const {
    glBindTexture(GL_TEXTURE_2D, this->ID);
}

std::string Texture::GetFilePath() const {
    return fileName;
}

int Texture::GetWidth() const {
    return width;
}

int Texture::GetHeight() const {
    return height;
}

unsigned int Texture::GetImageFormat() const {
    return imageFormat;
}

unsigned char* Texture::GetData() const {
    return data;
}

bool Texture::IsValid() const {
    if(this->data) {
        return true;
    }
    return false;
}
```

Explanation coming soon...

## Asset Manager

Now that we have the concept of a texture defined in ***Red Engine***, we will need something to maintain textures as well as other assets.

```c++
#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <unordered_map>
#include <string>

#include "./re/utils/logger.h"
#include "./re/rendering/texture.h"


class AssetManager {
  private:
    std::unordered_map<std::string, Texture*> textures;
    Logger *logger = nullptr;

    AssetManager();
  public:
    static AssetManager* GetInstance();

    void LoadTexture(const std::string &id, const std::string &filePath);

    Texture* GetTexture(const std::string &id);

    bool HasTexture(const std::string &id) const;
};

#endif //ASSET_MANAGER_H
```

```c++
#include "asset_manager.h"
#include <cassert>

AssetManager::AssetManager() : logger(Logger::GetInstance()) {}

AssetManager* AssetManager::GetInstance() {
    static AssetManager *instance = new AssetManager();
    return instance;
}

void AssetManager::LoadTexture(const std::string &id, const std::string &filePath) {
    Texture *texture = new Texture(filePath.c_str());
    assert(texture->IsValid() && "Failed to load texture!");
    if (HasTexture(id)) {
        logger->Warn("Already have texture, not loading...");
        return;
    }
    textures.emplace(id, texture);
}

Texture *AssetManager::GetTexture(const std::string &id) {
    assert(HasTexture(id) && "Failed to get texture!");
    return textures[id];
}

bool AssetManager::HasTexture(const std::string &id) const {
    return textures.count(id) > 0;
}
```

Explanation coming soon...
