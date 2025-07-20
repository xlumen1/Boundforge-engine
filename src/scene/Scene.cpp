#include "Scene.hpp"

namespace boundforge {

void Scene::addEntity(std::unique_ptr<Entity> entity) {
    entities.push_back(std::move(entity));
}

void Scene::removeEntity(Entity* entity) {
    entities.erase(
        std::remove_if(entities.begin(), entities.end(),
            [entity](const std::unique_ptr<Entity>& ptr) { return ptr.get() == entity; }),
        entities.end()
    );
}

void Scene::setPhysicsProvider(std::unique_ptr<IPhysicsProvider> provider) {
    physicsProvider = std::move(provider);
}

void Scene::update(float deltaTime) {
    // No physics for now
}

const std::vector<std::unique_ptr<Entity>>& Scene::getEntities() const {
    return entities;
}

}