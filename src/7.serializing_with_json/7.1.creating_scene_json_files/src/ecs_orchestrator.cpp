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

void ECSOrchestrator::RefreshEntitySignatureChanged(Entity entity) {
    ecSystemManager->EntitySignatureChanged(entity, entityManager->GetEnabledSignature(entity));
}

void ECSOrchestrator::PrepareSceneChange(const std::string& filePath) {
    sceneToChangeFilePath = filePath;
}

void ECSOrchestrator::ChangeToScene() {
    sceneManager->ChangeToScene(sceneToChangeFilePath);
    sceneToChangeFilePath.clear();
}

bool ECSOrchestrator::HasPreparedScene() const {
    return !sceneToChangeFilePath.empty();
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

void ECSOrchestrator::DeleteNode(Entity entity) {
    sceneManager->DeleteNode(entity);
}

bool ECSOrchestrator::IsNodeInScene(Entity entity) const {
    return sceneManager->IsNodeInScene(entity);
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

void ECSOrchestrator::OnSceneStartSystems(Scene* scene) {
    ecSystemManager->OnSceneStartSystems(scene);
}

void ECSOrchestrator::OnSceneEndSystems(Scene* scene) {
    ecSystemManager->OnSceneEndSystems(scene);
}
