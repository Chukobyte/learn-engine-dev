#include "scene_manager.h"

#include <cassert>

SceneManager* SceneManager::GetInstance() {
    static SceneManager *instance = new SceneManager();
    return instance;
}

void SceneManager::ChangeToEmptyScene() {
    if (currentScene) {
        for (auto &pair : currentScene->sceneNodes) {
            SceneNode *sceneNode = pair.second;
            delete sceneNode;
        }
        delete currentScene;
    }
    currentScene = new Scene{};
}

void SceneManager::AddRootNode(Entity rootEntity) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    assert(currentScene->rootNode == nullptr && "Current scene already has a root node!");
    if (IsNodeInScene(rootEntity)) {
        logger->Warn("Entity '%d' already in the scene!", rootEntity);
        return;
    }
    currentScene->sceneNodes.emplace(rootEntity, new SceneNode{ rootEntity });
}

void SceneManager::AddChildNode(Entity child, Entity parent) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    assert(IsNodeInScene(parent) && "Parent node is not in scene!");
    SceneNode *parentSceneNode = currentScene->sceneNodes[parent];
    SceneNode *childSceneNode = new SceneNode{ child, parentSceneNode };
    parentSceneNode->children.emplace_back(childSceneNode);
    currentScene->sceneNodes.emplace(child, childSceneNode);
}

void SceneManager::DeleteNode(Entity entity) {
    assert(currentScene != nullptr && "Current scene is NULL!");
    if (!IsNodeInScene(entity)) {
        logger->Warn("Attempted to delete entity '%d' which is not in the scene!", entity);
        return;
    }
    delete currentScene->sceneNodes[entity];
    currentScene->sceneNodes.erase(entity);
}

bool SceneManager::IsNodeInScene(Entity entity) const {
    assert(currentScene != nullptr && "Current scene is NULL!");
    return currentScene->sceneNodes.count(entity) > 0;
}
