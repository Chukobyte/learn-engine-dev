#pragma once

#include <set>

#include "../entity/entity_tag_cache.h"
#include "../../scene/scene.h"

const unsigned int MAX_SYSTEMS = 32;

class ECSystem {
  public:
    virtual void Initialize()  {
        enabled = true;
    }

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

    bool IsEnabled() const {
        return enabled;
    }

    bool HasEntity(Entity entity) const {
        return entities.count(entity) > 0;
    }

    // Event hooks
    virtual void Update(float deltaTime) {}
    virtual void PhysicsUpdate(float deltaTime) {}
    virtual void Render() {}
    virtual void OnSceneStart(Scene* scene) {}
    virtual void OnSceneEnd(Scene* scene) {}
    virtual void OnEntityTagsUpdated(Entity entity, const std::vector<std::string>& oldTags, const std::vector<std::string>& newTags) {
        entityTagCache.RemoveEntityTags(entity, oldTags);
        entityTagCache.AddEntityTags(entity, newTags);
    }
    virtual void OnEntityTagsRemoved(Entity entity, const std::vector<std::string>& tags) {}

  protected:
    bool enabled = false;
    std::set<Entity> entities;
    EntityTagCache entityTagCache;
};
