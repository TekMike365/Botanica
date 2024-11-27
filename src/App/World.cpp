#include "World.h"

namespace App
{
    World::World()
        : m_Size(8, 8, 8), m_Voxels(m_Size.x * m_Size.y * m_Size.z, 1), DataUpdated(true)
    {
    }

    void World::SetVoxel(glm::uvec3 pos, uint8_t type)
    {
        m_Voxels.at(pos.x + pos.y * m_Size.x + pos.z * m_Size.x * m_Size.y) = type;
        DataUpdated = true;
    }

    uint8_t World::GetVoxel(glm::uvec3 pos) const
    {
        return m_Voxels.at(pos.x + pos.y * m_Size.x + pos.z * m_Size.x * m_Size.y);
;
    }
}