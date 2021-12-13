# ECS Orchestrator

## ECS Orchestrator Class

```c++
#ifndef ECS_ORCHESTRATOR_H
#define ECS_ORCHESTRATOR_H

#include <vector>

#include "system/ec_system_manager.h"
#include "../scene/scene_manager.h"

class ECSOrchestrator {
  public:
    ~ECSOrchestrator();
    static ECSOrchestrator* GetInstance();

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
    void RegisterLoadedSceneNodeComponents();
    void AddRootNode(Entity rootEntity);
    void AddChildNode(Entity child, Entity parent);
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

    ECSOrchestrator();
    void RefreshEntitySignatureChanged(Entity entity);
    void DestroyEntity(Entity entity);
};

#endif //ECS_ORCHESTRATOR_H
```

```c++
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
    componentManager->EntityDestroyed(entity);
    ecSystemManager->EntityDestroyed(entity);
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
```

Now when you run the code, everything should function as normal but defined with the ecs system.  And just like every other section, you can find the source code in github [here](https://github.com/Chukobyte/learn-engine-dev/tree/main/src/1.foundation/6.entity_component_system/6.0.entity_component_system).  Now that we have an ecs let's now serialize scene and project properties with json.
