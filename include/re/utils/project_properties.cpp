#include "project_properties.h"

ProjectProperties* ProjectProperties::GetInstance() {
    static ProjectProperties *instance = new ProjectProperties();
    return instance;
}

std::string ProjectProperties::GetGameTitle() const {
    return gameTitle;
}

std::string ProjectProperties::GetInitialScenePath() const {
    return initialScenePath;
}

unsigned int ProjectProperties::GetWindowWidth() const {
    return windowWidth;
}

unsigned int ProjectProperties::GetWindowHeight() const {
    return windowHeight;
}

unsigned int ProjectProperties::GetTargetFPS() const {
    return targetFPS;
}

AssetConfigurations ProjectProperties::GetAssetConfigurations() {
    return assetConfigurations;
}

InputActionsConfigurations ProjectProperties::GetInputActionsConfigurations() {
    return inputActionsConfigurations;
}

void ProjectProperties::SetProjectProperties(const nlohmann::json& propertiesJson) {
    gameTitle = JsonHelper::Get<std::string>(propertiesJson, "game_title");
    initialScenePath = JsonHelper::Get<std::string>(propertiesJson, "initial_scene");
    nlohmann::json baseResolutionJson = JsonHelper::Get<nlohmann::json>(propertiesJson, "base_resolution");
    windowWidth = JsonHelper::Get<int>(baseResolutionJson, "width");
    windowHeight = JsonHelper::Get<int>(baseResolutionJson, "height");
    areColliderVisible = JsonHelper::Get<bool>(propertiesJson, "colliders_visible");
    targetFPS = JsonHelper::Get<unsigned int>(propertiesJson, "target_fps");
    nlohmann::json backgroundColorJson = JsonHelper::Get<nlohmann::json>(propertiesJson, "background_color");
    const int backgroundRed = JsonHelper::Get<int>(backgroundColorJson, "red");
    const int backgroundGreen = JsonHelper::Get<int>(backgroundColorJson, "green");
    const int backgroundBlue = JsonHelper::Get<int>(backgroundColorJson, "blue");
    backgroundClearColor = Color::NormalizedColor(backgroundRed, backgroundGreen, backgroundBlue);
    nlohmann::json assetsJsonArray = JsonHelper::Get<nlohmann::json>(propertiesJson, "assets");
    assetConfigurations = LoadProjectAssets(assetsJsonArray);
    nlohmann::json inputActionsJsonArray = JsonHelper::Get<nlohmann::json>(propertiesJson, "input_actions");
    inputActionsConfigurations = LoadProjectInputActions(inputActionsJsonArray);
}

AssetConfigurations ProjectProperties::LoadProjectAssets(const nlohmann::json& assetsJsonArray) {
    AssetConfigurations loadedAssetConfigurations;
    for (nlohmann::json assetJson : assetsJsonArray) {
        const std::string &assetType = JsonHelper::Get<std::string>(assetJson, "type");
        const std::string &assetsFilePath = JsonHelper::Get<std::string>(assetJson, "file_path");
        if (assetType == "texture") {
            const std::string &assetWrapS = JsonHelper::Get<std::string>(assetJson, "wrap_s");
            const std::string &assetWrapT = JsonHelper::Get<std::string>(assetJson, "wrap_t");
            const std::string &assetFilterMin = JsonHelper::Get<std::string>(assetJson, "filter_min");
            const std::string &assetFilterMax = JsonHelper::Get<std::string>(assetJson, "filter_max");
            loadedAssetConfigurations.textureConfigurations.emplace_back(TextureConfiguration{
                .filePath = assetsFilePath,
                .wrapS = assetWrapS,
                .wrapT = assetWrapT,
                .filterMin = assetFilterMin,
                .filterMax = assetFilterMax
            });
        } else if (assetType == "font") {
            const std::string &fontId = JsonHelper::Get<std::string>(assetJson, "uid");
            int fontSize = JsonHelper::Get<int>(assetJson, "size");
            loadedAssetConfigurations.fontConfigurations.emplace_back(FontConfiguration{
                .filePath = assetsFilePath,
                .uid = fontId,
                .size = fontSize
            });
        } else if (assetType == "music") {
            loadedAssetConfigurations.musicConfigurations.emplace_back(MusicConfiguration{
                .filePath = assetsFilePath
            });
        } else if (assetType == "sound") {
            loadedAssetConfigurations.soundConfigurations.emplace_back(SoundConfiguration{
                .filePath = assetsFilePath
            });
        }
    }
    return loadedAssetConfigurations;
}

InputActionsConfigurations ProjectProperties::LoadProjectInputActions(const nlohmann::json& inputActionsJsonArray) {
    InputActionsConfigurations loadInputActionsConfigurations;
    for (nlohmann::json inputActionJson : inputActionsJsonArray) {
        const std::string &actionName = JsonHelper::Get<std::string>(inputActionJson, "name");
        std::vector<std::string> inputActionValues;
        nlohmann::json actionValuesArray = JsonHelper::Get<nlohmann::json>(inputActionJson, "values");
        for (const auto &value : actionValuesArray) {
            inputActionValues.emplace_back(value);
        }
        InputConfiguration inputConfiguration{
            .name = actionName,
            .values = inputActionValues
        };
        loadInputActionsConfigurations.configurations.emplace_back(inputConfiguration);
    }
    return loadInputActionsConfigurations;
}
