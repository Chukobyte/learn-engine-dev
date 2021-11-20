#include "asset_manager.h"

AssetManager *AssetManager::instance = nullptr;

AssetManager::AssetManager() {
    logger = Logger::GetInstance();
}

AssetManager* AssetManager::GetInstance() {
    if (!instance) {
        instance = new AssetManager();
    }
    return instance;
}

void AssetManager::LoadTexture(const std::string &id, const std::string &filePath) {
    Texture *texture = new Texture(filePath.c_str());
    Logger::Assert(texture->IsValid(), "Failed to load texture!");
    if (HasTexture(id)) {
        logger->Warn("Already have texture, not loading...");
        return;
    }
    textures.emplace(id, texture);
}

Texture *AssetManager::GetTexture(const std::string &id) {
    Logger::Assert(HasTexture(id), "Failed to get texture '" + id + "'");
    return textures[id];
}

bool AssetManager::HasTexture(const std::string &id) const {
    return textures.count(id) > 0;
}
