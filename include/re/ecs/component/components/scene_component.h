#pragma once

#include <string>
#include <vector>

struct SceneComponent {
    std::string name;
    std::vector<std::string> tags;
    bool ignoreCamera = false;
};
