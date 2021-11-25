#ifndef PROJECT_PROPERTIES_H
#define PROJECT_PROPERTIES_H

#include <string>

class ProjectProperties {
  private:
    const std::string &gameTitle = "Test Game";
    const unsigned int windowWidth = 800;
    const unsigned int windowHeight = 600;

    ProjectProperties() = default;
  public:
    static ProjectProperties* GetInstance();

    std::string GetGameTitle() const;

    int GetWindowWidth() const;

    int GetWindowHeight() const;
};


#endif //PROJECT_PROPERTIES_H
