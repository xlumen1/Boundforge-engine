#pragma once
#include <vector>

namespace boundforge {

class PhysicsEntity;

class IPhysicsProvider {
public:
    virtual ~IPhysicsProvider() = default;
    virtual void update(float deltaTime, std::vector<PhysicsEntity*>& entities) = 0;
};

}