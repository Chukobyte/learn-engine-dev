#ifndef EC_SYSTEM_H
#define EC_SYSTEM_H

#include <set>

#include "../entity/entity.h"

const unsigned int MAX_SYSTEMS = 32;

class ECSystem {
  protected:
    bool enabled = false;
    std::set<Entity> entities;

  public:
    virtual void Initialize()  {}

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

    virtual void Update(float deltaTime) {}

    virtual void Render() {}
};

#endif //EC_SYSTEM_H
