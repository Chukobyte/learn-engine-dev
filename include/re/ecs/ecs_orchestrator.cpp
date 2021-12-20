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
    const std::function<void(const SceneNode& sceneNode)> registerNodeFunc = [this, &registerNodeFunc](const SceneNode& sceneNode) {
        RefreshEntitySignatureChanged(sceneNode.entity);
        SceneComponent sceneComponent = componentManager->GetComponentDefault<SceneComponent>(sceneNode.entity, {});
        ecSystemManager->OnEntityTagsUpdatedSystems(sceneNode.entity, {}, sceneComponent.tags);
        for (const SceneNode& childNode : sceneNode.children) {
            registerNodeFunc(childNode);
        }
    };
    registerNodeFunc(currentScene->rootNode);
}

void ECSOrchestrator::AddRootNode(Entity rootEntity) {
    sceneManager->AddRootNode(rootEntity);
    RefreshEntitySignatureChanged(rootEntity);
    SceneComponent sceneComponent = componentManager->GetComponentDefault<SceneComponent>(rootEntity, {});
    ecSystemManager->OnEntityTagsUpdatedSystems(rootEntity, {}, sceneComponent.tags);
}

void ECSOrchestrator::AddChildNode(Entity child, Entity parent) {
    sceneManager->AddChildNode(child, parent);
    RefreshEntitySignatureChanged(child);
    SceneComponent sceneComponent = componentManager->GetComponentDefault<SceneComponent>(child, {});
    ecSystemManager->OnEntityTagsUpdatedSystems(child, {}, sceneComponent.tags);
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
    SceneComponent sceneComponent = componentManager->GetComponentDefault<SceneComponent>(entity, {});
    ecSystemManager->EntityDestroyed(entity, sceneComponent.tags);
    componentManager->EntityDestroyed(entity);
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
