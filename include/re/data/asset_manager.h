#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <unordered_map>
#include <string>

#include <SDL2/SDL_mixer.h>

#include "../utils/logger.h"
#include "../rendering/texture.h"
#include "../rendering/font.h"
#include "../rendering/render_context.h"

class AssetManager {
  public:
    static AssetManager* GetInstance();
    // Texture
    void LoadTexture(const std::string &id, const std::string &filePath);
    Texture* GetTexture(const std::string &id);
    bool HasTexture(const std::string &id) const;
    // Font
    void LoadFont(const std::string &fontId, const std::string &fontPath, int size);
    Font* GetFont(const std::string &fontId);
    bool HasFont(const std::string &fontId) const;
    // Music
    void LoadMusic(const std::string &musicId, const std::string &musicPath);
    Mix_Music* GetMusic(const std::string &musicId);
    bool HasMusic(const std::string &musicId) const;
    // Sound
    void LoadSound(const std::string &soundId, const std::string &soundPath);
    Mix_Chunk* GetSound(const std::string &soundId);
    bool HasSound(const std::string &soundId) const;

  private:
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Font*> fonts;
    std::unordered_map<std::string, Mix_Music*> music;
    std::unordered_map<std::string, Mix_Chunk*> sounds;
    RenderContext *renderContext = nullptr;
    Logger *logger = nullptr;

    AssetManager();
};

#endif //ASSET_MANAGER_H
