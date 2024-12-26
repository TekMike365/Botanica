#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "Vector3D.h"

struct World
{
    Vector3D<uint32_t> VoxelIDs;

    World() = default;
    World(glm::uvec3 size)
        : VoxelIDs(size, 0) {}
};