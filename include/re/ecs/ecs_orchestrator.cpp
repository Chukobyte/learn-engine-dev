#include "ecs_orchestrator.h"

ECSOrchestrator::ECSOrchestrator() :
    ecSystemManager(new ECSystemManager()),
    entityManager(new EntityManager()),
    componentManager(ComponentManager::GetInstance()) {}

ECSOrchestrator::~ECSOrchestrator() {
    if (ecSystemManager) {
        delete ecSystemManager;
    }
    if (entityManager) {
        delete entityManager;
    }
    if (componentManager) {
        delete componentManager;
    }
}

ECSOrchestrator* ECSOrchestrator::GetInstance() {
    static ECSOrchestrator *instance = new ECSOrchestrator();
    return instance;
}

