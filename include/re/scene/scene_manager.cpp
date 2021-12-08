#include "scene_manager.h"

#include <cassert>

#include "scene_loader.h"
#include "../utils/file_helper.h"

SceneManager::SceneManager() : logger(Logger::GetInstance()) {}

SceneManager* SceneManager::GetInstance() {
    static SceneManager *instance = new SceneManager();
    return instance;
}

void SceneManager::ChangeToEmptyScene() {
    if (currentScene) {
//        for (auto &pair : currentScene->sceneNodes) {
//            SceneNode *sceneNode = pair.second;
//            delete sceneNode;
//        }
        delete currentScene;
    }
    currentScene = new Scene{};
}

void SceneManager::ChangeToScene(const std::string& filePath) {
    currentScene = SceneLoader::LoadSceneFile(filePath);
    assert(currentScene->rootNode.entity != NULL_ENTITY && "Scene root node is NULL!");
}

void SceneManager::ChangeToScene(Scene* newScene) {
    currentScene = newScene;
}

void SceneManager::AddRootNode(Entity rootEntity) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    assert(currentScene->rootNode.entity == NULL_ENTITY && "Current scene already has a root node!");
    if (IsNodeInScene(rootEntity)) {
        logger->Warn("Entity '%d' already in the scene!", rootEntity);
        return;
    }
    currentScene->sceneNodes.emplace(rootEntity, SceneNode{ rootEntity });
}

void SceneManager::AddChildNode(Entity child, Entity parent) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    assert(IsNodeInScene(parent) && "Parent node is not in scene!");
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
