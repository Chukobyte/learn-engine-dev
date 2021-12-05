#ifndef EC_SYSTEM_MANAGER_H
#define EC_SYSTEM_MANAGER_H

#include <cassert>
#include <unordered_map>

#include "ec_system.h"
#include "../component/component.h"
#include "../../utils/logger.h"

class ECSystemManager {
  private:
    std::unordered_map<const char*, ComponentSignature> signatures{};
    std::unordered_map<const char*, ECSystem*> systems{};
    Logger *logger = nullptr;

  public:
    ECSystemManager() {
        logger = Logger::GetInstance();
    }

    template<typename T>
    T* GetSystem() {
        const char *typeName = typeid(T).name();
        assert(HasSystem<T>() && "System used before registered.");
        return (T*) systems[typeName];
    }

    template<typename T>
    T* RegisterSystem() {
        const char *typeName = typeid(T).name();

        assert(!HasSystem<T>() && "Registering system more than once.");

        auto *system = new T();
        system->Enable();
        systems.insert({typeName, system});
        return system;
    }

    template<typename T>
    bool HasSystem() {
        const char *typeName = typeid(T).name();
        return systems.find(typeName) != systems.end();
    }

    template<typename T>
    ECSystem* GetEntitySystem() {
        const char *typeName = typeid(T).name();
        assert(HasSystem<T>() && "System used before registered.");
        return systems[typeName];
    }

    template<typename T>
    void EnableSystem() {
        ECSystem *system = GetSystem<T>();
        if (!system->IsEnabled()) {
            system->Enable();
        } else {
            logger->Warn("System already enabled!");
        }
    }

    template<typename T>
    void DisableSystem() {
        ECSystem *system = GetSystem<T>();
        if (system->IsEnabled()) {
            system->Disable();
        } else {
            logger->Warn("System already disabled!");
        }
    }

    template<typename T>
    void SetSignature(ComponentSignature signature) {
        const char* typeName = typeid(T).name();

        assert(HasSystem<T>() && "System used before registered.");

        signatures.insert({typeName, signature});
    }

    template<typename T>
    ComponentSignature GetSignature() {
        const char* typeName = typeid(T).name();

        assert(HasSystem<T>() && "System hasn't been registered!");

        return signatures[typeName];
    }

    void InitializeAllSystems() {
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->Initialize();
        }
    }

    void EntityDestroyed(Entity entity) {
        for (auto const& pair : systems) {
            auto const& system = pair.second;
            system->UnregisterEntity(entity);
        }
    }

    void EntitySignatureChanged(Entity entity, ComponentSignature entitySignature) {
        // Notify each system that an entity's signature changed
        for (auto const& pair : systems) {
            auto const& type = pair.first;
            auto const& system = pair.second;
            auto const& systemSignature = signatures[type];

            // Entity signature matches system signature register
            if ((entitySignature & systemSignature) == systemSignature) {
                system->RegisterEntity(entity);
            } else {
                system->UnregisterEntity(entity);
            }
        }
    }
};

#endif //EC_SYSTEM_MANAGER_H
