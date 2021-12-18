#pragma once

#include <string>
#include <sstream>

class Helper {
  public:
    template<typename T>
    static T& Clamp(T &value, T min, T max) {
        value = std::min(value, min);
        value = std::max(value, max);
        return value;
    }

    template<typename EnumClass, typename CastType = int>
    static bool CompareEnumClass(EnumClass flagA, EnumClass flagB) {
        return ((CastType) flagA & (CastType) flagB) == (CastType) flagB;
    }

    static std::string GetNumberFromEndOfString(const std::string& text) {
        std::string currentEndNumberString;
        for (auto it = text.crbegin() ; it != text.crend(); ++it) {
            const char nameCharacter =  *it;
            if (isdigit(nameCharacter)) {
                currentEndNumberString = std::string(1, nameCharacter) + currentEndNumberString;
            } else {
                break;
            }
        }
        return currentEndNumberString;
    }

    static std::string RemoveNumberFromEndOfString(const std::string& text) {
        const std::string& endStringText = GetNumberFromEndOfString(text);
        if (!endStringText.empty()) {
            std::string currentText = text;
            currentText.resize(currentText.size() - endStringText.size());
            return currentText;
        }
        return text;
    }

    static unsigned int ConvertStringToUnsignedInt(const std::string& text) {
        std::istringstream reader(text);
        unsigned int value;
        reader >> value;
        return value;
    }
};
