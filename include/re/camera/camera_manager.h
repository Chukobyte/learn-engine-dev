#pragma once

#include "../utils/singleton.h"

#include "camera2d.h"

class CameraManager : public Singleton<CameraManager> {
  public:
    CameraManager(singleton) {}
    Camera2D GetCurrentCamera();
    void UpdateCurrentCamera2D(Camera2D updatedCamera);
    void ResetCamera2D();

  private:
    Camera2D camera2D;
};
