# Building An ECS

## Entity

An `Entity` will represented as an `unsigned int`.

```c++
#pragma once

using Entity = unsigned int;

const Entity MAX_ENTITIES = 20000;
const Entity NULL_ENTITY = 0;
```

## Component

Components will add functionality for an entity.

### Component Class

```c++
#pragma once

#include <bitset>

const std::uint32_t MAX_COMPONENT_TYPES = 32;

using ComponentType = std::uint32_t;
using ComponentSignature = std::bitset<MAX_COMPONENT_TYPES>;
```

### Component Array

```c++
#pragma once

#include <unordered_map>
#include <cassert>

#include "../entity/entity.h"
#include "component.h"

class IComponentArray {
  public:
    virtual ~IComponentArray() = default;
    virtual void EntityDestroyed(Entity entity) = 0;
};

template<typename T>
class ComponentArray : public IComponentArray {
  public:
    void InsertNewData(Entity entity, T component) {
        assert(entityToIndexMap.find(entity) == entityToIndexMap.end() && "Component added to same entity more than once!");

        size_t newIndex = size;
        entityToIndexMap[entity] = newIndex;
        indexToEntityMap[newIndex] = entity;
        components[newIndex] = component;

        size++;
    }

    void UpdateData(Entity entity, T component) {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Component hasn't been added!");

        components[entityToIndexMap[entity]] = component;
    }

    void RemoveData(Entity entity) {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Removing non-existent component!");

        // Copy element at end into deleted element's place to maintain array density
        size_t indexOfRemovedEntity = entityToIndexMap[entity];
        size_t indexOfLastElement = size - 1;
        components[indexOfRemovedEntity] = components[indexOfLastElement];

        // Update map to point to moved spot
        Entity entityOfLastElement = indexToEntityMap[indexOfLastElement];
        entityToIndexMap[entityOfLastElement] = indexOfRemovedEntity;
        indexToEntityMap[indexOfRemovedEntity] = entityOfLastElement;

        entityToIndexMap.erase(entity);
        indexToEntityMap.erase(indexOfLastElement);

        size--;
    }

    T& GetData(Entity entity) {
        assert(entityToIndexMap.find(entity) != entityToIndexMap.end() && "Retrieving non-existent component!");

        return components[entityToIndexMap[entity]];
    }

    bool HasData(Entity entity) {
        return entityToIndexMap.find(entity) != entityToIndexMap.end();
    }

    void EntityDestroyed(Entity entity) override {
        if (entityToIndexMap.find(entity) != entityToIndexMap.end()) {
            RemoveData(entity);
        }
    }

  private:
    std::array<T, MAX_ENTITIES> components;
    std::unordered_map<Entity, size_t> entityToIndexMap;
    std::unordered_map<size_t, Entity> indexToEntityMap;
    size_t size = 0;
};
```

## System

Systems will process components for an entity based on a signature.

```c++
#pragma once

#include <set>

#include "../entity/entity.h"
#include "../../scene/scene.h"

const unsigned int MAX_SYSTEMS = 32;

class ECSystem {
  protected:
    bool enabled = false;
    std::set<Entity> entities;

  public:
    virtual void Initialize()  {
        enabled = true;
    }

    virtual void RegisterEntity(Entity entity) {
        entities.insert(entity);
    }

    virtual void UnregisterEntity(Entity entity) {
        entities.erase(entity);
    }

    virtual void Enable() {
        enabled = true;
    }

    virtual void Disable() {
        enabled = false;
    }

    bool IsEnabled() {
        return enabled;
    }

    // Event hooks
    virtual void Update(float deltaTime) {}
    virtual void PhysicsUpdate(float deltaTime) {}
    virtual void Render() {}
    virtual void OnSceneStart(Scene* scene) {}
    virtual void OnSceneEnd(Scene* scene) {}
};
```

## Scene

Entities will be placed in a scene.

```c++
#pragma once

#include <vector>
#include <unordered_map>

#include "../ecs/entity/entity.h"
#include "../utils/helper.h"

struct SceneNode {
    Entity entity = NULL_ENTITY;
    Entity parent = NULL_ENTITY;
    std::vector<SceneNode> children = {};
};

struct Scene {
    SceneNode rootNode = {};
    std::unordered_map<Entity, SceneNode> sceneNodes = {};
};
```
