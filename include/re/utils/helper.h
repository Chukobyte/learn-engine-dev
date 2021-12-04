#ifndef HELPER_H
#define HELPER_H

class Helper {
  public:
    template<typename T>
    static T& Clamp(T &value, T min, T max) {
        value = std::min(value, min);
        value = std::max(value, max);
        return value;
    }
};

#endif //HELPER_H
