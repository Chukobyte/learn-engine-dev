#pragma once

#include "../utils/singleton.h"

#include <vector>

#include "system/ec_system_manager.h"
#include "../scene/scene_manager.h"

class ECSOrchestrator : public Singleton<ECSOrchestrator> {
  public:
    ECSOrchestrator(singleton);
    ~ECSOrchestrator();

    void DestroyEntity(Entity entity);
    void DeleteEntitiesQueuedForDeletion();

    // Entity
    Entity CreateEntity() {
        return entityManager->CreateEntity();
    }

    // Component
    template<typename T>
    void RegisterComponent() {
        componentManager->RegisterComponent<T>();
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        componentManager->AddComponent<T>(entity, component);
        auto signature = entityManager->GetEnabledSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetSignature(entity, signature);
        entityManager->SetEnabledSignature(entity, signature);
        RefreshEntitySignatureChanged(entity);
    }

    template<typename T>
    void UpdateComponent(Entity entity, T component) {
        componentManager->UpdateComponent(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        componentManager->RemoveComponent<T>(entity);
        auto signature = entityManager->GetSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), false);
        entityManager->SetSignature(entity, signature);

        auto enabledSignature = entityManager->GetEnabledSignature(entity);
        enabledSignature.set(componentManager->GetComponentType<T>(), false);
        entityManager->SetEnabledSignature(entity, enabledSignature);
        ecSystemManager->EntitySignatureChanged(entity, enabledSignature);
    }

    template<typename T>
    void EnableComponent(Entity entity) {
        auto signature = entityManager->GetEnabledSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetEnabledSignature(entity, signature);
        ecSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void DisableComponent(Entity entity) {
        auto signature = entityManager->GetEnabledSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), false);
        entityManager->SetEnabledSignature(entity, signature);
        ecSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    bool IsComponentEnabled(Entity entity) {
        auto signature = entityManager->GetEnabledSignature(entity);
        return (GetComponentType<T>() & signature) == signature;
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return componentManager->GetComponent<T>(entity);
    }

    template<typename T>
    ComponentType GetComponentType() {
        return componentManager->GetComponentType<T>();
    }

    template<typename T>
    bool HasComponent(Entity entity) {
        return componentManager->HasComponent<T>(entity);
    }


    // EC System
    template<typename T>
    T* RegisterSystem(ComponentSignature initialComponentSignature = {}, ECSystemRegistration ecSystemRegistration = ECSystemRegistration::NONE) {
        T *system = ecSystemManager->RegisterSystem<T>(ecSystemRegistration);
        SetSystemSignature<T>(initialComponentSignature);
        return system;
    }

    template<typename T>
    T* GetSystem() {
        return ecSystemManager->GetSystem<T>();
    }

    template<typename T>
    bool HasSystem() {
        return ecSystemManager->HasSystem<T>();
    }

    template<typename T>
    void SetSystemSignature(ComponentSignature signature) {
        ecSystemManager->SetSignature<T>(signature);
    }

    template<typename T>
    ComponentSignature GetSystemSignature() {
        return ecSystemManager->GetSignature<T>();
    }

    // Event Hooks
    void UpdateSystems(float deltaTime);
    void PhysicsUpdateSystems(float deltaTime);
    void RenderSystems();
    void OnSceneStartSystems();
    void OnSceneEndSystems();

    // Scene
    void PrepareSceneChange(const std::string& filePath);
    void ChangeToScene();
    void DestroyScene();
    bool HasSceneToCreate() const;
    bool HasSceneToDestroy() const;

    // Triggered when an entity enters a scene
    void RegisterLoadedSceneNodeComponents(); // From scene json
    void AddRootNode(Entity rootEntity); // From script
    void AddChildNode(Entity child, Entity parent); // from script

    bool IsNodeInScene(Entity entity) const;
    void QueueDestroyEntity(Entity entity);
    void DestroyQueuedEntities();
    Scene* GetCurrentScene();

  private:
    ECSystemManager *ecSystemManager = nullptr;
    EntityManager *entityManager = nullptr;
    ComponentManager *componentManager = nullptr;
    SceneManager *sceneManager = nullptr;
    std::string sceneToChangeFilePath;
    bool shouldDestroySceneNextFrame = false;
    std::vector<Entity> entitiesQueuedForDeletion;

    void RefreshEntitySignatureChanged(Entity entity);
};
