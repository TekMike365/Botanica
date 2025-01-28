#include "pch.h"
#include "Plant.h"

Plant::Plant(std::shared_ptr<World> world, glm::uvec3 pos)
    : m_World(world), m_Pos(pos)
{
    // TODO: initialize DNA
    Init();
}

Plant::Plant(std::shared_ptr<World> world, glm::uvec3 pos, const PlantDNA &dna)
    : m_World(world), m_Pos(pos), m_DNA(dna)
{
    Init();
}

void Plant::Init()
{
    glm::uvec3 rootPos(m_Pos);
    glm::uvec3 stemPos(m_Pos.x, m_Pos.y + 1, m_Pos.z);
    glm::uvec3 leafPos(m_Pos.x, m_Pos.y + 2, m_Pos.z);

    if (m_World->GetVoxel(rootPos) != VoxelTypeSoil ||
        m_World->GetVoxel(stemPos) != VoxelTypeAir ||
        m_World->GetVoxel(leafPos) != VoxelTypeAir)
    {
        m_IsAlive = false;
        return;
    }

    // :D
    m_RootPositions.emplace_back(rootPos);
    m_StemPositions.emplace_back(stemPos);
    m_LeafPositions.emplace_back(leafPos);

    m_World->SetVoxel(rootPos, VoxelTypeRoot);
    m_World->SetVoxel(stemPos, VoxelTypeStem);
    m_World->SetVoxel(leafPos, VoxelTypeLeaf);
    m_IsAlive = true;
}
