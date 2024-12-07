#include "World.h"

namespace App
{
    World::World(glm::uvec3 size, uint32_t init)
        : m_Size(size), m_Voxels(GetVoxelCount(), init), DataUpdated(true)
    {
    }

    void World::SetVoxel(glm::uvec3 pos, uint32_t type)
    {
        m_Voxels.at(pos.x + pos.y * m_Size.x + pos.z * m_Size.x * m_Size.y) = type;
        DataUpdated = true;
    }

    uint32_t World::GetVoxel(glm::uvec3 pos) const
    {
        return m_Voxels.at(pos.x + pos.y * m_Size.x + pos.z * m_Size.x * m_Size.y);
    }
}