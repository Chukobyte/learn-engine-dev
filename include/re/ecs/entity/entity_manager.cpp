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
