#ifndef ECS_ORCHESTRATOR_H
#define ECS_ORCHESTRATOR_H

#include "./re/ecs/system/ec_system_manager.h"
#include "./re/ecs/entity/entity_manager.h"
#include "./re/ecs/component/component_manager.h"
#include "./re/scene/scene_manager.h"

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
        auto signature = entityManager->GetSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetSignature(entity, signature);
        RefreshEntitySignature(entity);
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
        ecSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    void EnableComponent(Entity entity) {
        auto signature = entityManager->GetSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), true);
        entityManager->SetSignature(entity, signature);
        RefreshEntitySignature(entity);
    }

    template<typename T>
    void DisableComponent(Entity entity) {
        auto signature = entityManager->GetSignature(entity);
        signature.set(componentManager->GetComponentType<T>(), false);
        entityManager->SetSignature(entity, signature);
        ecSystemManager->EntitySignatureChanged(entity, signature);
    }

    template<typename T>
    bool IsComponentEnabled(Entity entity) {
        auto signature = entityManager->GetSignature(entity);
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

    void UpdateSystems(float deltaTime);
    void RenderSystems();

    // Scene
    void ChangeToEmptyScene();
    void ChangeToScene(const std::string& filePath);
    void AddRootNode(Entity rootEntity);
    void AddChildNode(Entity child, Entity parent);
    void DeleteNode(Entity entity);
    bool IsNodeInScene(Entity entity) const;

  private:
    ECSystemManager *ecSystemManager = nullptr;
    EntityManager *entityManager = nullptr;
    ComponentManager *componentManager = nullptr;
    SceneManager *sceneManager = nullptr;

    ECSOrchestrator();
    void RefreshEntitySignature(Entity entity);
};

#endif //ECS_ORCHESTRATOR_H
