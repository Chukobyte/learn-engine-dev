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
