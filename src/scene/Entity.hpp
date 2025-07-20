#pragma once
#include <memory>
#include "../physics/PhysicsEntity.hpp"

namespace boundforge {

class Entity {
public:
    std::unique_ptr<PhysicsEntity> physics;
    Entity(std::unique_ptr<PhysicsEntity> phys) : physics(std::move(phys)) {}
    virtual ~Entity() = default;
};

}