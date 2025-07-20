#pragma once
#include <glm/glm.hpp>

namespace boundforge {

class PhysicsEntity {
public:
    glm::vec3 position;
    glm::vec3 rotation;
    glm::vec3 linear_velocity;
    glm::vec3 angular_velocity;
    float mass;

    PhysicsEntity(const glm::vec3& pos, float m)
        : position(pos), velocity(0.0f), mass(m) {}
    virtual ~PhysicsEntity() = default;
};

}