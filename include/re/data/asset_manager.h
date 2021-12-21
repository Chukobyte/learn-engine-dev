#pragma once

#include <unordered_map>
#include <string>

#include "../audio/audio.h"
#include "../utils/logger.h"
#include "re/project_properties.h"
#include "../rendering/texture.h"
#include "../rendering/font.h"
#include "../rendering/render_context.h"

class AssetManager {
  public:
    static AssetManager* GetInstance();
    // Texture
    void LoadTexture(const std::string &id, const std::string &filePath, const std::string &wrapS = "clamp_to_border", const std::string &wrapT = "clamp_to_border", const std::string &filterMin = "nearest", const std::string &filterMag = "nearest");
    Texture* GetTexture(const std::string &id);
    bool HasTexture(const std::string &id) const;
    // Font
    void LoadFont(const std::string &fontId, const std::string &fontPath, int size);
    Font* GetFont(const std::string &fontId);
    bool HasFont(const std::string &fontId) const;
    // Music
    void LoadMusic(const std::string &musicId, const std::string &musicPath);
    Music* GetMusic(const std::string &musicId);
    bool HasMusic(const std::string &musicId) const;
    // Sound
    void LoadSound(const std::string &soundId, const std::string &soundPath);
    SoundEffect* GetSound(const std::string &soundId);
    bool HasSound(const std::string &soundId) const;
    std::unordered_map<std::string, SoundEffect*> GetAllSounds();
    // All
    void LoadProjectConfigurations(AssetConfigurations assetConfigurations);

  private:
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Font*> fonts;
    std::unordered_map<std::string, Music*> music;
    std::unordered_map<std::string, SoundEffect*> soundEffects;
    RenderContext *renderContext = nullptr;
    Logger *logger = nullptr;

    AssetManager();
};
