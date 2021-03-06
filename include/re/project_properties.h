#pragma once

#include "utils/singleton.h"

#include <string>
#include <vector>

#include "rendering/color.h"
#include "utils/json_helper.h"

struct TextureConfiguration {
    std::string filePath;
    std::string wrapS = "clamp_to_border";
    std::string wrapT = "clamp_to_border";
    std::string filterMin = "nearest";
    std::string filterMag = "nearest";
};

struct FontConfiguration {
    std::string filePath;
    std::string uid;
    int size;
};

struct MusicConfiguration {
    std::string filePath;
};

struct SoundConfiguration {
    std::string filePath;
};

struct AssetConfigurations {
    std::vector<TextureConfiguration> textureConfigurations;
    std::vector<FontConfiguration> fontConfigurations;
    std::vector<MusicConfiguration> musicConfigurations;
    std::vector<SoundConfiguration> soundConfigurations;
};

struct InputConfiguration {
    std::string name;
    std::vector<std::string> values;
};

struct InputActionsConfigurations {
    std::vector<InputConfiguration> configurations;
};

class ProjectProperties : public Singleton<ProjectProperties> {
  public:
    Color backgroundClearColor = Color::NormalizedColor(50, 50, 50);
    bool areColliderVisible = false;

    ProjectProperties(singleton) {}
    std::string GetGameTitle() const;
    std::string GetInitialScenePath() const;
    unsigned int GetWindowWidth() const;
    unsigned int GetWindowHeight() const;
    unsigned int GetTargetFPS() const;
    AssetConfigurations GetAssetConfigurations();
    InputActionsConfigurations GetInputActionsConfigurations();

    void SetProjectProperties(const nlohmann::json& propertiesJson);
private:
    std::string gameTitle;
    std::string initialScenePath;
    unsigned int windowWidth = 800;
    unsigned int windowHeight = 600;
    unsigned int targetFPS = 60;
    AssetConfigurations assetConfigurations;
    InputActionsConfigurations inputActionsConfigurations;

    AssetConfigurations LoadProjectAssets(const nlohmann::json& assetsJsonArray);
    InputActionsConfigurations LoadProjectInputActions(const nlohmann::json& inputActionsJsonArray);
};
