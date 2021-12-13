# Managers

## Entity Manager

```c++
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <array>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cassert>

#include "entity.h"
#include "../component/component.h"

class EntityManager {
  public:
    static EntityManager* GetInstance();
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void DeleteEntitiesQueuedForDeletion();
    unsigned int GetAliveEntities();
    void SetSignature(Entity entity, ComponentSignature signature);
    void SetEnabledSignature(Entity entity, ComponentSignature signature);
    void ResetEnabledSignature(Entity entity);
    ComponentSignature GetSignature(Entity entity);
    ComponentSignature GetEnabledSignature(Entity entity);

  private:
    unsigned int entityIdCounter = 1;  // Starts at 1 as 0 is invalid
    unsigned int livingEntityCounter = 0;
    std::queue<Entity> availableEntityIds;
    std::array<ComponentSignature, MAX_ENTITIES> signatures;
    std::array<ComponentSignature, MAX_ENTITIES> enabledSignatures;
    std::vector<Entity> entitiesToDelete;

    EntityManager() = default;
    Entity GetUniqueEntityId();
};

#endif //ENTITY_MANAGER_H
```

```c++
#include "entity_manager.h"

#include "../../utils/helper.h"

EntityManager* EntityManager::GetInstance() {
    static EntityManager* instance = new EntityManager();
    return instance;
}

Entity EntityManager::CreateEntity() {
    assert(livingEntityCounter < MAX_ENTITIES && "Too many entities to create!");

    livingEntityCounter++;

    return GetUniqueEntityId();
}

void EntityManager::DestroyEntity(Entity entity) {
    entitiesToDelete.insert(entitiesToDelete.end(), 1, entity);
    livingEntityCounter--;
}

void EntityManager::DeleteEntitiesQueuedForDeletion() {
    for (Entity entity : entitiesToDelete) {
        signatures[entity].reset();
        enabledSignatures[entity].reset();
        availableEntityIds.push(entity);
    }
    entitiesToDelete.clear();
}

unsigned int EntityManager::GetAliveEntities() {
    return livingEntityCounter;
}

void EntityManager::SetSignature(Entity entity, ComponentSignature signature) {
    signatures[entity] = signature;
}

ComponentSignature EntityManager::GetSignature(Entity entity) {
    return signatures[entity];
}

ComponentSignature EntityManager::GetEnabledSignature(Entity entity) {
    return enabledSignatures[entity];
}

void EntityManager::SetEnabledSignature(Entity entity, ComponentSignature signature) {
    enabledSignatures[entity] = signature;
}

void EntityManager::ResetEnabledSignature(Entity entity) {
    enabledSignatures[entity] = signatures[entity];
}

Entity EntityManager::GetUniqueEntityId() {
    if (availableEntityIds.empty()) {
        availableEntityIds.push(entityIdCounter);
        entityIdCounter++;
    }
    Entity newEntityId = availableEntityIds.front();
    availableEntityIds.pop();
    return newEntityId;
}
```

## Component Manager

```c++
#ifndef COMPONENT_MANAGER_H
#define COMPONENT_MANAGER_H

#include <unordered_map>
#include <memory>
#include <iostream>

#include "component.h"
#include "component_array.h"

class ComponentManager {
  private:
    std::unordered_map<const char*, ComponentType> componentTypes;
    std::unordered_map<const char*, IComponentArray*> componentArrays;
    unsigned int componentIndex = 0;

    ComponentManager() = default;

    template<typename T>
    ComponentArray<T>* GetComponentArray() {
        const char *typeName = typeid(T).name();

        assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered before use.");

        return (ComponentArray<T>*) componentArrays[typeName];
    }

  public:
    static ComponentManager* GetInstance();

    template<typename T>
    void RegisterComponent() {
        const char *typeName = typeid(T).name();

        assert(componentTypes.find(typeName) == componentTypes.end() && "Registering component type more than once!");

        componentTypes.insert({typeName, componentIndex});

        componentArrays.insert({typeName, new ComponentArray<T>()});

        componentIndex++;
    }

    template<typename T>
    ComponentType GetComponentType() {
        const char *typeName = typeid(T).name();

        assert(componentTypes.find(typeName) != componentTypes.end() && "Component not registered!");

        return componentTypes[typeName];
    }

    template<typename T>
    void AddComponent(Entity entity, T component) {
        GetComponentArray<T>()->InsertNewData(entity, component);
    }

    template<typename T>
    void UpdateComponent(Entity entity, T component) {
        GetComponentArray<T>()->UpdateData(entity, component);
    }

    template<typename T>
    void RemoveComponent(Entity entity) {
        GetComponentArray<T>()->RemoveData(entity);
    }

    template<typename T>
    T& GetComponent(Entity entity) {
        return GetComponentArray<T>()->GetData(entity);
    }

    template<typename T>
    bool HasComponent(Entity entity) {
        return GetComponentArray<T>()->HasData(entity);
    }

    void EntityDestroyed(Entity entity);
};

#endif //COMPONENT_MANAGER_H
```

```c++
#include "component_manager.h"

ComponentManager* ComponentManager::GetInstance() {
    static ComponentManager *instance = new ComponentManager();
    return instance;
}

void ComponentManager::EntityDestroyed(Entity entity) {
    for (auto const &pair : componentArrays) {
        auto const &component = pair.second;
        component->EntityDestroyed(entity);
    }
}
```

## EC System Manager

```c++
#ifndef ENTITY_MANAGER_H
#define ENTITY_MANAGER_H

#include <array>
#include <vector>
#include <queue>
#include <unordered_map>
#include <cassert>

#include "entity.h"
#include "../component/component.h"

class EntityManager {
  public:
    static EntityManager* GetInstance();
    Entity CreateEntity();
    void DestroyEntity(Entity entity);
    void DeleteEntitiesQueuedForDeletion();
    unsigned int GetAliveEntities();
    void SetSignature(Entity entity, ComponentSignature signature);
    void SetEnabledSignature(Entity entity, ComponentSignature signature);
    void ResetEnabledSignature(Entity entity);
    ComponentSignature GetSignature(Entity entity);
    ComponentSignature GetEnabledSignature(Entity entity);

  private:
    unsigned int entityIdCounter = 1;  // Starts at 1 as 0 is invalid
    unsigned int livingEntityCounter = 0;
    std::queue<Entity> availableEntityIds;
    std::array<ComponentSignature, MAX_ENTITIES> signatures;
    std::array<ComponentSignature, MAX_ENTITIES> enabledSignatures;
    std::vector<Entity> entitiesToDelete;

    EntityManager() = default;
    Entity GetUniqueEntityId();
};

#endif //ENTITY_MANAGER_H
```

```c++
#include "entity_manager.h"

#include "../../utils/helper.h"

EntityManager* EntityManager::GetInstance() {
    static EntityManager* instance = new EntityManager();
    return instance;
}

Entity EntityManager::CreateEntity() {
    assert(livingEntityCounter < MAX_ENTITIES && "Too many entities to create!");

    livingEntityCounter++;

    return GetUniqueEntityId();
}

void EntityManager::DestroyEntity(Entity entity) {
    entitiesToDelete.insert(entitiesToDelete.end(), 1, entity);
    livingEntityCounter--;
}

void EntityManager::DeleteEntitiesQueuedForDeletion() {
    for (Entity entity : entitiesToDelete) {
        signatures[entity].reset();
        enabledSignatures[entity].reset();
        availableEntityIds.push(entity);
    }
    entitiesToDelete.clear();
}

unsigned int EntityManager::GetAliveEntities() {
    return livingEntityCounter;
}

void EntityManager::SetSignature(Entity entity, ComponentSignature signature) {
    signatures[entity] = signature;
}

ComponentSignature EntityManager::GetSignature(Entity entity) {
    return signatures[entity];
}

ComponentSignature EntityManager::GetEnabledSignature(Entity entity) {
    return enabledSignatures[entity];
}

void EntityManager::SetEnabledSignature(Entity entity, ComponentSignature signature) {
    enabledSignatures[entity] = signature;
}

void EntityManager::ResetEnabledSignature(Entity entity) {
    enabledSignatures[entity] = signatures[entity];
}

Entity EntityManager::GetUniqueEntityId() {
    if (availableEntityIds.empty()) {
        availableEntityIds.push(entityIdCounter);
        entityIdCounter++;
    }
    Entity newEntityId = availableEntityIds.front();
    availableEntityIds.pop();
    return newEntityId;
}
```
