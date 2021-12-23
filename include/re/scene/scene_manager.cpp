#include "scene_manager.h"

#include <cassert>

#include "scene_loader.h"
#include "../utils/file_helper.h"

SceneManager::SceneManager(singleton) : logger(Logger::GetInstance()) {}

void SceneManager::ChangeToEmptyScene() {
    if (currentScene) {
        delete currentScene;
    }
    currentScene = new Scene{};
}

void SceneManager::ChangeToScene(const std::string& filePath) {
    currentScene = SceneLoader::LoadSceneFile(filePath);
    assert(currentScene->rootNode.entity != NULL_ENTITY && "Scene root node is NULL!");
}

void SceneManager::AddRootNode(Entity rootEntity) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    if (IsNodeInScene(rootEntity)) {
        logger->Warn("Attempting to add as root node Entity '%d' which is already in the scene!", rootEntity);
        return;
    }
    currentScene->sceneNodes.emplace(rootEntity, SceneNode{ rootEntity });
}

void SceneManager::AddChildNode(Entity child, Entity parent) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    assert(IsNodeInScene(parent) && "Parent node is not in scene!");
    if (IsNodeInScene(child)) {
        logger->Warn("Attempting to add child entity '%d' which is already in the scene!  Passing in parent = %d", child, parent);
        return;
    }
    SceneNode parentSceneNode = currentScene->sceneNodes[parent];
    SceneNode childSceneNode = SceneNode{ child, currentScene->sceneNodes[parent].entity };
    currentScene->sceneNodes[parent].children.emplace_back(childSceneNode);
    currentScene->sceneNodes.emplace(child, childSceneNode);
}

void SceneManager::DeleteNode(Entity entity) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    if (!IsNodeInScene(entity)) {
        logger->Warn("Attempted to delete entity '%d' which is not in the scene!", entity);
        return;
    }
    currentScene->sceneNodes.erase(entity);
}

bool SceneManager::IsNodeInScene(Entity entity) const {
    assert(currentScene != nullptr && "Current scene is NULL!");
    return currentScene->sceneNodes.count(entity) > 0;
}

Scene* SceneManager::GetCurrentScene() {
    assert(currentScene != nullptr && "Attempted to get null scene!");
    return currentScene;
}
