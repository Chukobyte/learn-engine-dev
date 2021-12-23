#include "component_manager.h"

void ComponentManager::EntityDestroyed(Entity entity) {
    for (auto const &pair : componentArrays) {
        auto const &component = pair.second;
        component->EntityDestroyed(entity);
    }
}
