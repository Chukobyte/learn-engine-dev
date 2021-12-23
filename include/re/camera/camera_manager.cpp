#include "camera_manager.h"

Camera2D CameraManager::GetCurrentCamera() {
    return camera2D;
}

void CameraManager::UpdateCurrentCamera2D(Camera2D updatedCamera) {
    camera2D = updatedCamera;
}

void CameraManager::ResetCamera2D() {
    camera2D = Camera2D{};
}
