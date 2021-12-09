#ifndef HELPER_H
#define HELPER_H

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

    static unsigned int ConvertStringToUnsignedInt(const std::string& text) {
        std::istringstream reader(text);
        unsigned int value;
        reader >> value;
        return value;
    }
};

#endif //HELPER_H
