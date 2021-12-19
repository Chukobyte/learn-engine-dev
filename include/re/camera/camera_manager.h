#pragma once

#include "camera2d.h"

class CameraManager {
  public:
    static CameraManager* GetInstance();
    Camera2D GetCurrentCamera();
    void UpdateCurrentCamera2D(Camera2D updatedCamera);
    void ResetCamera2D();

  private:
    Camera2D camera2D;

    CameraManager() = default;
};