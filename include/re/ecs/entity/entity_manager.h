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
