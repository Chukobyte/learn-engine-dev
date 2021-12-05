#include "component_manager.h"

ComponentManager* ComponentManager::GetInstance() {
    static ComponentManager *instance = new ComponentManager();
    return instance;
}
