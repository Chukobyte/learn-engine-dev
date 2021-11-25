#include "project_properties.h"

ProjectProperties* ProjectProperties::GetInstance() {
    static ProjectProperties *instance = new ProjectProperties();
    return instance;
}

std::string ProjectProperties::GetGameTitle() const {
    return gameTitle;
}

int ProjectProperties::GetWindowWidth() const {
    return windowWidth;
}

int ProjectProperties::GetWindowHeight() const {
    return windowHeight;
}
