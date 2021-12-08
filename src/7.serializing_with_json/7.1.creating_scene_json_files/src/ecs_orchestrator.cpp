#include "ecs_orchestrator.h"

ECSOrchestrator::ECSOrchestrator() :
    ecSystemManager(new ECSystemManager()),
    entityManager(EntityManager::GetInstance()),
    componentManager(ComponentManager::GetInstance()),
    sceneManager(SceneManager::GetInstance()) {}

ECSOrchestrator::~ECSOrchestrator() {
    if (ecSystemManager) {
        delete ecSystemManager;
    }
    if (entityManager) {
        delete entityManager;
    }
}

ECSOrchestrator* ECSOrchestrator::GetInstance() {
    static ECSOrchestrator *instance = new ECSOrchestrator();
    return instance;
}

void ECSOrchestrator::RefreshEntitySignature(Entity entity) {
    if (sceneManager->IsNodeInScene(entity)) {
        ecSystemManager->EntitySignatureChanged(entity, entityManager->GetSignature(entity));
    }
}

//void ECSOrchestrator::ChangeToEmptyScene() {
//    sceneManager->ChangeToEmptyScene();
//}

void ECSOrchestrator::ChangeToScene(const std::string& filePath) {
    sceneManager->ChangeToScene(filePath);
}

void ECSOrchestrator::AddNodesToScene() {
    Scene* currentScene = sceneManager->GetCurrentScene();
    AddRootNode(currentScene->rootNode.entity);
    for (SceneNode childSceneNode : currentScene->rootNode.children) {
        AddChildNode(childSceneNode.entity, childSceneNode.parent);
    }
}

void ECSOrchestrator::AddRootNode(Entity rootEntity) {
    sceneManager->AddRootNode(rootEntity);
    RefreshEntitySignature(rootEntity);
}

void ECSOrchestrator::AddChildNode(Entity child, Entity parent) {
    sceneManager->AddChildNode(child, parent);
    RefreshEntitySignature(child);
}

void ECSOrchestrator::DeleteNode(Entity entity) {
    sceneManager->DeleteNode(entity);
}

bool ECSOrchestrator::IsNodeInScene(Entity entity) const {
    return sceneManager->IsNodeInScene(entity);
}

void ECSOrchestrator::UpdateSystems(float deltaTime) {
    ecSystemManager->UpdateSystems(deltaTime);
}

void ECSOrchestrator::RenderSystems() {
    ecSystemManager->RenderSystems();
}