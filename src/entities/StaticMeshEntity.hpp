#pragma once
#include "../scene/Entity.hpp"
#include <vector>
#include <glm/glm.hpp>

namespace boundforge {

class StaticMeshEntity : public Entity {
public:
    std::vector<glm::vec3> vertices;
    std::vector<glm::ivec3> indices; // Each triangle as 3 indices
    int materialIndex;

    StaticMeshEntity(
        const std::vector<glm::vec3>& verts,
        const std::vector<glm::ivec3>& tris,
        int matIdx
    ) : Entity(nullptr), vertices(verts), indices(tris), materialIndex(matIdx) {}
};

}