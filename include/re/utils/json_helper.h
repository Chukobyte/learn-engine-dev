#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <iostream>
#include <json/json.hpp>
#include <cassert>

#include <fstream>

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

#endif //JSON_HELPER_H
