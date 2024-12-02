#include "World.h"

namespace App
{
    World::World()
        : m_Size(8, 8, 8), m_Voxels(m_Size.x * m_Size.y * m_Size.z, 1), DataUpdated(true)
    {
    }

    World::World(glm::uvec3 size)
        : m_Size(size), m_Voxels(m_Size.x * m_Size.y * m_Size.z, 1), DataUpdated(true)
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