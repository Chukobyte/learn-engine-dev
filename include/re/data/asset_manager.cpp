#include "asset_manager.h"
#include <cassert>

AssetManager::AssetManager() : renderContext(RenderContext::GetInstance()), logger(Logger::GetInstance()) {}

AssetManager* AssetManager::GetInstance() {
    static AssetManager *instance = new AssetManager();
    return instance;
}

void AssetManager::LoadTexture(const std::string &id, const std::string &filePath) {
    if (HasTexture(id)) {
        logger->Warn("Already have texture, not loading...");
        return;
    }
    Texture *texture = new Texture(filePath.c_str());
    assert(texture->IsValid() && "Failed to load texture!");
    textures.emplace(id, texture);
}

Texture *AssetManager::GetTexture(const std::string &id) {
    assert(HasTexture(id) && "Failed to get texture!");
    return textures[id];
}

bool AssetManager::HasTexture(const std::string &id) const {
    return textures.count(id) > 0;
}

void AssetManager::LoadFont(const std::string &fontId, const std::string &fontPath, int size) {
    if (HasFont(fontId)) {
        logger->Warn("Already have font, not loading!");
        return;
    }
    Font *font = new Font(renderContext->freeTypeLibrary, fontPath.c_str(), size);
    assert(font->IsValid() && "Failed to load font!");
    fonts.emplace(fontId, font);
}

Font *AssetManager::GetFont(const std::string &fontId) {
    assert(HasFont(fontId) && "Failed to get font!");
    return fonts[fontId];
}

bool AssetManager::HasFont(const std::string &fontId) const {
    return fonts.count(fontId) > 0;
}

void AssetManager::LoadMusic(const std::string &musicId, const std::string &musicPath) {

}

Mix_Music *AssetManager::GetMusic(const std::string &musicId) {
    return nullptr;
}

bool AssetManager::HasMusic(const std::string &musicId) const {
    return false;
}

void AssetManager::LoadSound(const std::string &soundId, const std::string &soundPath) {

}

Mix_Chunk *AssetManager::GetSound(const std::string &soundId) {
    return nullptr;
}

bool AssetManager::HasSound(const std::string &soundId) const {
    return false;
}