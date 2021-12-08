#include "entity_manager.h"

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
    assert(entity < MAX_ENTITIES && "Entity out of range!");

    signatures[entity].reset();

    entitiesToDelete.insert(entitiesToDelete.end(), 1, entity);
    livingEntityCounter--;
}

void EntityManager::DeleteEntitiesQueuedForDeletion() {
    for (Entity entity : entitiesToDelete) {
        availableEntityIds.push(entity);
    }
    entitiesToDelete.clear();
}

unsigned int EntityManager::GetAliveEntities() {
    return livingEntityCounter;
}

void EntityManager::SetSignature(Entity entity, ComponentSignature signature) {
    assert(entity < MAX_ENTITIES && "Entity out of range!");

    signatures[entity] = signature;
}

ComponentSignature EntityManager::GetSignature(Entity entity) {
    assert(entity < MAX_ENTITIES && "Entity out of range!");

    return signatures[entity];
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
