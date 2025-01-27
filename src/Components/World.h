#pragma once

#include <cstdint>
#include <glm/glm.hpp>

#include "Vector3D.h"

enum VoxelType
{
    VoxelTypeAir = 0,
    VoxelTypeRoot = 1,
    VoxelTypeWater = 2,
    VoxelTypeFruit = 3,
    VoxelTypeStem = 4,
    VoxelTypeLeaf = 5,
    VoxelTypeSoil = 6
};

class World
{
    friend class RenderingLayer;

public:
    World() = default;
    World(glm::uvec3 size)
        : m_Size(size), m_VoxelIDs(size, 0) {}

    inline void SetVoxel(glm::uvec3 pos, uint32_t voxelType) { m_VoxelIDs[pos] = voxelType; }
    inline glm::uvec3 GetSize() const { return m_Size; }

private:
    glm::uvec3 m_Size;
    Vector3D<uint32_t> m_VoxelIDs;
};