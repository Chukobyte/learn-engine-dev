#ifndef JSON_HELPER_H
#define JSON_HELPER_H

#include <iostream>
#include <json/json.hpp>
#include <cassert>

#include <fstream>

class JsonHelper {
public:
    static bool HasKey(const nlohmann::json &json, const std::string &key) {
        try {
            json.at(key);
            return true;
        } catch (nlohmann::json::type_error &te) {
        } catch (nlohmann::json::out_of_range &oor) {
        } catch (std::exception &e) {
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
