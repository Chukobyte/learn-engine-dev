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

Texture::Texture(unsigned int width, unsigned int height, unsigned int colorValue) :
    wrapS(GL_CLAMP_TO_BORDER),
    wrapT(GL_CLAMP_TO_BORDER),
    filterMin(GL_LINEAR),
    filterMag(GL_LINEAR),
    nrChannels(4),
    width(width),
    height(height),
    logger(Logger::GetInstance()) {
    data = new unsigned char[width * height * sizeof(unsigned char)];
    for(int i = 0; i < (int)(width * height * sizeof(unsigned char)); i++) {
        data[i] = colorValue;
    }
    if(IsValid()) {
        Generate();
    } else {
        logger->Error("Failed to load color texture");
    }
}

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
