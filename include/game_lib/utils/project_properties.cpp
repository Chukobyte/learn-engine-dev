#include "project_properties.h"

ProjectProperties* ProjectProperties::GetInstance() {
    static ProjectProperties *instance = new ProjectProperties();
    return instance;
}

std::string ProjectProperties::GetGameTitle() const {
    return gameTitle;
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
