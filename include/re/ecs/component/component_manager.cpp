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
