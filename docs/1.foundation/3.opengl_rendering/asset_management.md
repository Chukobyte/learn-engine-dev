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
  public:
    Texture(const char* filePath, unsigned int wrapS = GL_CLAMP_TO_BORDER, unsigned int wrapT = GL_CLAMP_TO_BORDER, unsigned int filterMin = GL_NEAREST, unsigned int filterMag = GL_NEAREST);
    Texture(const char* filePath, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMag);
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
    unsigned int internalFormat = GL_RGBA;
    unsigned int imageFormat = GL_RGBA;
    // configuration
    unsigned int wrapS = GL_CLAMP_TO_BORDER;
    unsigned int wrapT = GL_CLAMP_TO_BORDER;
    unsigned int filterMin = GL_NEAREST;
    unsigned int filterMag = GL_NEAREST;

    void Generate();
    unsigned int GetWrapFromString(const std::string &wrap) const;
    unsigned int GetFilterFromString(const std::string &filter) const;
};

#endif //TEXTURE_H
```

```c++
#include "texture.h"

#include <stb_image/stb_image.h>

Texture::Texture(const char* filePath, unsigned int wrapS, unsigned int wrapT, unsigned int filterMin, unsigned int filterMag) :
    wrapS(wrapS),
    wrapT(wrapT),
    filterMin(filterMin),
    filterMag(filterMag),
    logger(Logger::GetInstance()) {
    fileName = std::string(filePath);
    // load image, create texture, and generate mipmaps
    stbi_set_flip_vertically_on_load(false);
    data = stbi_load(filePath, &width, &height, &nrChannels, 0);
    if(IsValid()) {
        Generate();
    } else {
        logger->Error("Texture failed to load at: %s", filePath);
    }
}

Texture::Texture(const char* filePath, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMag) :
    Texture(
        filePath,
        GetWrapFromString(wrapS),
        GetWrapFromString(wrapT),
        GetFilterFromString(filterMin),
        GetFilterFromString(filterMag)
    ) {}

Texture::~Texture() {
    stbi_image_free(data);
    data = nullptr;
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
    glGenTextures(1, &ID);
    Bind();
    glTexImage2D(GL_TEXTURE_2D, 0, internalFormat, width, height, 0, imageFormat, GL_UNSIGNED_BYTE, data);
    glGenerateMipmap(GL_TEXTURE_2D);
    // Wrap and filter modes
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_S, wrapS);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_WRAP_T, wrapT);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MIN_FILTER, filterMin);
    glTexParameteri(GL_TEXTURE_2D, GL_TEXTURE_MAG_FILTER, filterMag);
    // Unbind texture
    glBindTexture(GL_TEXTURE_2D, 0);
}

unsigned int Texture::GetWrapFromString(const std::string &wrap) const {
    if (wrap == "clamp_to_border") {
        return GL_CLAMP_TO_BORDER;
    } else if (wrap == "clamp_to_edge") {
        return GL_CLAMP_TO_EDGE;
    } else if (wrap == "repeat") {
        return GL_REPEAT;
    } else if (wrap == "mirrored_repeat") {
        return GL_MIRRORED_REPEAT;
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
    glBindTexture(GL_TEXTURE_2D, ID);
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
    if(data) {
        return true;
    }
    return false;
}
```

The `Texture` class has quite a few member variables.  `fileName` is just the file path of the texture file.  `data` is the texture data in memory.  `width` and `height` are self explanatory.  `nrChannels` represent the number of color channels.  `internalFormat` specifies the number of color components in the texture.  `imageFormat` is set based on the number of color channels for the loaded texture.

`wrapS` and `wrapT` refers to the texture wrapping mode applied to the S and T axis.  Below are the 4 possible values:

- `GL_REPEAT`: The default behavior for textures. Repeats the texture image.

- `GL_MIRRORED_REPEAT`: Same as GL_REPEAT but mirrors the image with each repeat.

- `GL_CLAMP_TO_EDGE`: Clamps the coordinates between 0 and 1. The result is that higher coordinates become clamped to the edge, resulting in a stretched edge pattern.

- `GL_CLAMP_TO_BORDER`: Coordinates outside the range are now given a user-specified border color.

`filterMin` and `filterMag` refers to the texture filtering mode.  Below are the possible values:

- `GL_NEAREST`: OpenGL selects the texel that center is closest to the texture coordinate.  Ideal for pixel art games.

- `GL_LINEAR`: OpenGL takes an interpolated value from the texture coordinate's neighboring texel.

The `Initialize` function is called by both constructors.  Texture data is loaded by stb_image library with the following line:

```c++
data = stbi_load(filePath, &width, &height, &nrChannels, 0);
```

If the texture is loaded successfully, the `Generate` function is called.  This function configures the texture for OpenGL.  The only other function to point out is the `Bind` function which we'll get into once we start rendering textures.

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
  public:
    static AssetManager* GetInstance();
    void LoadTexture(const std::string &id, const std::string &filePath);
    Texture* GetTexture(const std::string &id);
    bool HasTexture(const std::string &id) const;

  private:
    std::unordered_map<std::string, Texture*> textures;
    Logger *logger = nullptr;

    AssetManager();
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

The `AssetManager` class is a singleton with 3 functions to pay attention to.  `LoadTexture` loads a new instance of the `Texture` class and places it in the textures map.  `GetTexture` just retrieves a texture by id from the map.  And `HasTexture` checks to see if the texture is stored by `AssetManager`.  Other assets will follow the same 3 function pattern as textures and will be discussed in a later section.
