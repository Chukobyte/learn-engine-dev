#include "entity_manager.h"

#include "../../utils/helper.h"

#include<ctype.h>

EntityManager* EntityManager::GetInstance() {
    static EntityManager* instance = new EntityManager();
    return instance;
}

Entity EntityManager::CreateEntity(std::string name) {
    assert(livingEntityCounter < MAX_ENTITIES && "Too many entities to create!");
    const Entity newEntity = GetUniqueEntityId();
    if (nameToEntityMap.count(name) > 0) {
        name = GetUniqueEntityName(name);
    }
    nameToEntityMap.emplace(name, newEntity);

    livingEntityCounter++;

    return newEntity;
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

std::string EntityManager::GetUniqueEntityName(std::string name) const {
    unsigned int uniqueId = 2;
    const std::string& numberAtTheEndText = Helper::GetNumberFromEndOfString(name);
    if (!numberAtTheEndText.empty()) {
        name.resize(name.size() - numberAtTheEndText.size());
        const unsigned int numberAtTheEnd = Helper::ConvertStringToUnsignedInt(numberAtTheEndText);
        uniqueId = numberAtTheEnd + 1;
    }
    return name + std::to_string(uniqueId);
}
