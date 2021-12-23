#include "asset_manager.h"

#include <cassert>

AssetManager::AssetManager(singleton) : renderContext(RenderContext::GetInstance()), logger(Logger::GetInstance()) {}

void AssetManager::LoadTexture(const std::string &id, const std::string &filePath, const std::string &wrapS, const std::string &wrapT, const std::string &filterMin, const std::string &filterMag) {
    if (HasTexture(id)) {
        logger->Warn("Already have texture, not loading...");
        return;
    }
    Texture *texture = new Texture(filePath.c_str(), wrapS, wrapT, filterMin, filterMag);
    assert(texture->IsValid() && "Failed to load texture!");
    textures.emplace(id, texture);
}

Texture *AssetManager::GetTexture(const std::string &id) {
    if (!HasTexture(id)) {
        logger->Error("texture id = '%s'", id.c_str());
    }
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
    if (HasMusic(musicId)) {
        logger->Warn("Already has music, not loading!");
        return;
    }
    Music *newMusic = new Music(musicPath);
    assert(newMusic->IsValid() && "Failed to load music!");
    music.emplace(musicId, newMusic);
}

Music *AssetManager::GetMusic(const std::string &musicId) {
    assert(HasMusic(musicId) && "Failed to get music!");
    return music[musicId];
}

bool AssetManager::HasMusic(const std::string &musicId) const {
    return music.count(musicId) > 0;
}

void AssetManager::LoadSound(const std::string &soundId, const std::string &soundPath) {
    if (HasSound(soundId)) {
        logger->Warn("Already have sound, not loading!");
        return;
    }
    SoundEffect *soundEffect = new SoundEffect(soundPath);
    assert(soundEffect->IsValid() && "Failed to load sound effect!");
    soundEffects.emplace(soundId, soundEffect);
}

SoundEffect *AssetManager::GetSound(const std::string &soundId) {
    assert(HasSound(soundId) && "Failed to get sound effect!");
    return soundEffects[soundId];
}

bool AssetManager::HasSound(const std::string &soundId) const {
    return soundEffects.count(soundId) > 0;
}

std::unordered_map<std::string, SoundEffect *> AssetManager::GetAllSounds() {
    return soundEffects;
}

void AssetManager::LoadProjectConfigurations(AssetConfigurations assetConfigurations) {
    for (TextureConfiguration textureConfiguration : assetConfigurations.textureConfigurations) {
        LoadTexture(textureConfiguration.filePath,
                    textureConfiguration.filePath,
                    textureConfiguration.wrapS,
                    textureConfiguration.wrapT,
                    textureConfiguration.filterMin,
                    textureConfiguration.filterMag);
    }
    for (FontConfiguration fontConfiguration : assetConfigurations.fontConfigurations) {
        LoadFont(fontConfiguration.uid, fontConfiguration.filePath, fontConfiguration.size);
    }
    for (MusicConfiguration musicConfiguration : assetConfigurations.musicConfigurations) {
        LoadMusic(musicConfiguration.filePath, musicConfiguration.filePath);
    }
    for (SoundConfiguration soundConfiguration : assetConfigurations.soundConfigurations) {
        LoadSound(soundConfiguration.filePath, soundConfiguration.filePath);
    }
}
