# Parsing JSON Properties File

## Nlohmann Json

***Red Engine*** will use the [nlohmann json](https://github.com/nlohmann/json) library to help with parsing json files.

## Json Helper

The `JsonHelper` class will be used to help validate json fields and the `JsonFileHelper` to load json files.

```c++
#pragma once

#include <iostream>
#include <fstream>
#include <cassert>

#include <json/json.hpp>

class JsonHelper {
  public:
    template<typename T>
    static T Get(const nlohmann::json& json, const std::string& key) {
        if (json.contains(key)) {
            return json.at(key);
        }
        std::cerr << "Key '" << key << "' doesn't exist!" << std::endl;
        assert(false && "Key doesn't exist in json!");
        return T(); // Not reached, just added to have returns for all code paths
    }

    template<typename T>
    static T GetDefault(const nlohmann::json& json, const std::string& key, T defaultValue) {
        if (json.contains(key)) {
            return json.at(key);
        }
        return defaultValue;
    }
};

class JsonFileHelper {
  public:
    static nlohmann::json LoadJsonFile(const std::string& filePath) {
        std::ifstream i(filePath);
        nlohmann::json json;
        i >> json;
        return json;
    }

};
```

## Project Properties

Example of `properties.json`.

```json
{
  "game_title": "2D Test",
  "initial_scene": "scenes/main.sscn",
  "base_resolution": {
    "width": 800,
    "height": 600
  },
  "colliders_visible": true,
  "target_fps": 60,
  "background_color": {
    "red": 50,
    "green": 50,
    "blue": 50
  },
  "assets": [
    {
      "type": "texture",
      "file_path": "assets/images/melissa_walk_animation.png",
      "wrap_s": "clamp_to_border",
      "wrap_t": "clamp_to_border",
      "filter_min": "nearest",
      "filter_max": "nearest"
    },
    {
      "type": "font",
      "uid": "verdana-32",
      "file_path": "assets/fonts/verdana.ttf",
      "size": 32
    },
    {
      "type": "music",
      "file_path": "assets/audio/music/test_music.wav"
    },
    {
      "type": "sound",
      "file_path": "assets/audio/sound/test_sound_effect.wav"
    }
  ],
  "input_actions": [
    {
      "name": "quit",
      "values": [
        "esc"
      ]
    },
    {
      "name": "move_left",
      "values": [
        "left"
      ]
    },
    {
      "name": "move_right",
      "values": [
        "right"
      ]
    }
  ]
}
```

## Game Engine Class

Add a new function to load properties json file.

```c++
bool GameEngine::LoadProjectProperties() {
    const char* projectPropertiesPath = "properties.json";
    if (!FileHelper::DoesFileExist(projectPropertiesPath)) {
        logger->Error("Unable to find properties file '%s'", projectPropertiesPath);
        return false;
    }
    projectProperties->SetProjectProperties(JsonFileHelper::LoadJsonFile(projectPropertiesPath));
    return true;
}
```

We should now call this before everything else in the `Initialize` function.

```c++
bool GameEngine::Initialize() {
    logger->Debug("Initializing...");
    if (!LoadProjectProperties()) {
        logger->Error("Failed to load project properties!");
        return false;
    }
    if (!InitializeSDL()) {
        logger->Error("Failed to initialize SDL!");
        return false;
    }
    if (!InitializeAudio()) {
        logger->Error("Failed to initialize audio!");
        return false;
    }
    if (!InitializeRendering()) {
        logger->Error("Failed to initialize rendering!");
        return false;
    }
    if (!InitializeInput()) {
        logger->Error("Failed to initialize input!");
        return false;
    }
    if (!InitializeECS()) {
        logger->Error("Failed to initialize ECS!");
        return false;
    }
    assetManager->LoadProjectConfigurations(projectProperties->GetAssetConfigurations());
    logger->Info("%s Engine v%s", engineContext->GetEngineName(), engineContext->GetEngineVersion());
    engineContext->SetRunning(true);

    // Temp play music
    AudioHelper::PlayMusic("test_music");
    return true;
}
```

Now that we are initializing the project properties by passing in the data loading in from the json properties file.  Don't forget to remove the `LoadTexture`, `LoadMusic`, `LoadSound`, and `AddAction` function calls as these are no longer needed in `game_engine.cpp`.  The source code for this section can be found [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/7.serializing_with_json/7.0.parsing_json_properties_file).
