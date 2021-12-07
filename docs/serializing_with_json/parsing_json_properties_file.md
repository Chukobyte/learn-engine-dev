# Parsing JSON Properties File

## Nlohmann Json

***Red Engine*** will use the [nlohmann json](https://github.com/nlohmann/json) library to help with parsing json files.

## Json Helper

The `JsonHelper` class will be used to help validate json fields and the `JsonFileHelper` to load json files.

```c++
#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <iostream>
#include <fstream>
#include <cassert>

#include <json/json.hpp>

class JsonHelper {
  public:
    static bool HasKey(const nlohmann::json& json, const std::string& key) {
        try {
            json.at(key);
            return true;
        } catch (nlohmann::json::type_error &te) {
        } catch (nlohmann::json::out_of_range &oor) {
        }
        return false;
    }

    template<typename T>
    static T Get(const nlohmann::json& json, const std::string& key) {
        if (HasKey(json, key)) {
            return json.at(key);
        }
        std::cerr << "Key '" << key << "' doesn't exist!" << std::endl;
        assert(false && "Key doesn't exist in json!");
        return T(); // Not reached, just added to have returns for all code paths
    }

    template<typename T>
    static T GetDefault(const nlohmann::json& json, const std::string& key, T defaultValue) {
        if (HasKey(json, key)) {
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

#endif //JSON_HELPER_H
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
      "file_path": "assets/images/seika_idle.png",
      "wrap_s": "clamp_to_border",
      "wrap_t": "clamp_to_border",
      "filter_min": "nearest",
      "filter_mag": "nearest"
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
        "esc",
        "joystick_button_back"
      ]
    },
    {
      "name": "mouse_left_click",
      "values": [
        "mb_left"
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
