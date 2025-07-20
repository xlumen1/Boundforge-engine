#pragma once
#include <vector>
#include <memory>
#include "Entity.hpp"
#include "../physics/IPhysicsProvider.hpp"

namespace boundforge {

class Scene {
    std::vector<std::unique_ptr<Entity>> entities;
    std::unique_ptr<IPhysicsProvider> physicsProvider;
public:
    void addEntity(std::unique_ptr<Entity> entity);
    void removeEntity(Entity* entity);
    void setPhysicsProvider(std::unique_ptr<IPhysicsProvider> provider);
    void update(float deltaTime);

    // Declaration only, no inline definition!
    const std::vector<std::unique_ptr<Entity>>& getEntities() const;
};

}