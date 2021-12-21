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

void ECSOrchestrator::DeleteEntitiesQueuedForDeletion() {
    entityManager->DeleteEntitiesQueuedForDeletion();
}

void ECSOrchestrator::RefreshEntitySignatureChanged(Entity entity) {
    ecSystemManager->EntitySignatureChanged(entity, entityManager->GetEnabledSignature(entity));
}

void ECSOrchestrator::PrepareSceneChange(const std::string& filePath) {
    sceneToChangeFilePath = filePath;
    shouldDestroySceneNextFrame = true;
}

void ECSOrchestrator::ChangeToScene() {
    sceneManager->ChangeToScene(sceneToChangeFilePath);
    sceneToChangeFilePath.clear();
}

void ECSOrchestrator::DestroyScene() {
    shouldDestroySceneNextFrame = false;
}

bool ECSOrchestrator::HasSceneToCreate() const {
    return !sceneToChangeFilePath.empty() && !shouldDestroySceneNextFrame;
}

bool ECSOrchestrator::HasSceneToDestroy() const {
    return shouldDestroySceneNextFrame;
}

void ECSOrchestrator::RegisterLoadedSceneNodeComponents() {
    Scene* currentScene = sceneManager->GetCurrentScene();
    RefreshEntitySignatureChanged(currentScene->rootNode.entity);
    for (SceneNode childSceneNode : currentScene->rootNode.children) {
        RefreshEntitySignatureChanged(childSceneNode.entity);
    }
}

void ECSOrchestrator::AddRootNode(Entity rootEntity) {
    sceneManager->AddRootNode(rootEntity);
    RefreshEntitySignatureChanged(rootEntity);
}

void ECSOrchestrator::AddChildNode(Entity child, Entity parent) {
    sceneManager->AddChildNode(child, parent);
    RefreshEntitySignatureChanged(child);
}

void ECSOrchestrator::QueueDestroyEntity(Entity entity) {
    entitiesQueuedForDeletion.emplace_back(entity);
}

void ECSOrchestrator::DestroyQueuedEntities() {
    for (Entity entity : entitiesQueuedForDeletion) {
        DestroyEntity(entity);
    }
    entitiesQueuedForDeletion.clear();
}

void ECSOrchestrator::DestroyEntity(Entity entity) {
    sceneManager->DeleteNode(entity);
    entityManager->DestroyEntity(entity);
    componentManager->EntityDestroyed(entity);
    ecSystemManager->EntityDestroyed(entity, {});
}

bool ECSOrchestrator::IsNodeInScene(Entity entity) const {
    return sceneManager->IsNodeInScene(entity);
}

Scene* ECSOrchestrator::GetCurrentScene() {
    return sceneManager->GetCurrentScene();
}

void ECSOrchestrator::UpdateSystems(float deltaTime) {
    ecSystemManager->UpdateSystems(deltaTime);
}

void ECSOrchestrator::PhysicsUpdateSystems(float deltaTime) {
    ecSystemManager->PhysicsUpdateSystems(deltaTime);
}

void ECSOrchestrator::RenderSystems() {
    ecSystemManager->RenderSystems();
}

void ECSOrchestrator::OnSceneStartSystems() {
    ecSystemManager->OnSceneStartSystems(sceneManager->GetCurrentScene());
}

void ECSOrchestrator::OnSceneEndSystems() {
    ecSystemManager->OnSceneEndSystems(sceneManager->GetCurrentScene());
}
