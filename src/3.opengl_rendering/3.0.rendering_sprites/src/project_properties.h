#ifndef PROJECT_PROPERTIES_H
#define PROJECT_PROPERTIES_H

#include <string>

#include "./game_lib/rendering/color.h"

class ProjectProperties {
  private:
    const std::string gameTitle = "Test Game";
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;
    const unsigned int targetFPS = 60;
    ProjectProperties() = default;

  public:
    const Color backgroundClearColor = Color(50.f / 255.0f, 50.f / 255.0f, 50.f / 255.0f);

    static ProjectProperties* GetInstance();

    std::string GetGameTitle() const;

    unsigned int GetWindowWidth() const;

    unsigned int GetWindowHeight() const;

    unsigned int GetTargetFPS() const;
};


#endif //PROJECT_PROPERTIES_H
