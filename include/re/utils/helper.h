#pragma once

#include <string>
#include <sstream>

#define GENERATE_ENUM_CLASS_OPERATORS(EnumClass) \
inline EnumClass operator | (EnumClass lhs, EnumClass rhs) { \
    using T = std::underlying_type_t <EnumClass>; \
    return static_cast<EnumClass>(static_cast<T>(lhs) | static_cast<T>(rhs)); \
} \
 \
inline EnumClass& operator |= (EnumClass& lhs, EnumClass rhs) { \
    lhs = lhs | rhs; \
    return lhs; \
} \
inline EnumClass operator & (EnumClass lhs, EnumClass rhs) { \
    using T = std::underlying_type_t <EnumClass>; \
    return static_cast<EnumClass>(static_cast<T>(lhs) & static_cast<T>(rhs)); \
} \
\
inline EnumClass& operator &= (EnumClass& lhs, EnumClass rhs) { \
    lhs = lhs & rhs; \
    return lhs; \
}

class Helper {
  public:
    template<typename T>
    static T& Clamp(T &value, T min, T max) {
        value = std::min(value, min);
        value = std::max(value, max);
        return value;
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
