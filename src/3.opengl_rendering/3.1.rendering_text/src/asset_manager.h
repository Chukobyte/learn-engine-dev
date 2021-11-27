#ifndef ASSET_MANAGER_H
#define ASSET_MANAGER_H

#include <unordered_map>
#include <string>

#include "./re/utils/logger.h"
#include "./re/rendering/texture.h"
#include "./re/rendering/font.h"
#include "./re/rendering/render_context.h"


class AssetManager {
  private:
    std::unordered_map<std::string, Texture*> textures;
    std::unordered_map<std::string, Font*> fonts;
    RenderContext *renderContext = nullptr;
    Logger *logger = nullptr;

    AssetManager();
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
};


#endif //ASSET_MANAGER_H
