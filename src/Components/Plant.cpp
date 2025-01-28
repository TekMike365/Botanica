#include "pch.h"
#include "Plant.h"

Plant::Plant(std::shared_ptr<World> world, glm::uvec3 pos)
    : m_World(world), m_Pos(pos)
{
    // TODO: initialize DNA
    srand(time(NULL));

    std::array<int, 4> growthChoice;
    for (int &i : growthChoice)
        i = rand() % m_DNA.MAX_VALUE;

    std::array<int, 26> leafGrowAction;
    for (int &i : leafGrowAction)
        i = rand() % m_DNA.MAX_VALUE;

    std::array<int, 26> rootGrowAction;
    for (int &i : rootGrowAction)
        i = rand() % m_DNA.MAX_VALUE;

    m_DNA = {
        .GrowthChoice = growthChoice,
        .RootGrowAction = rootGrowAction,
        .LeafGrowAction = leafGrowAction,
    };

    Init();
}

Plant::Plant(std::shared_ptr<World> world, glm::uvec3 pos, const PlantDNA &dna)
    : m_World(world), m_Pos(pos), m_DNA(dna)
{
    Init();
}

Plant::~Plant()
{
    for (const auto &p : m_FruitPositions)
        m_World->SetVoxel(p, VoxelTypeAir);
    for (const auto &p : m_LeafPositions)
        m_World->SetVoxel(p, VoxelTypeAir);
    for (const auto &p : m_StemPositions)
        m_World->SetVoxel(p, VoxelTypeAir);
    for (const auto &p : m_RootPositions)
        m_World->SetVoxel(p, VoxelTypeSoil);
}

void Plant::Mine()
{
}

void Plant::Grow()
{
}

bool Plant::IsAlive() const
{
    if (m_RootPositions.size() == 0 || m_LeafPositions.size() == 0 || m_StemPositions.size() == 0)
        return false;

    return true;
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
        return;
    }

    // :D
    m_RootPositions.emplace_back(rootPos);
    m_StemPositions.emplace_back(stemPos);
    m_LeafPositions.emplace_back(leafPos);

    m_World->SetVoxel(rootPos, VoxelTypeRoot);
    m_World->SetVoxel(stemPos, VoxelTypeStem);
    m_World->SetVoxel(leafPos, VoxelTypeLeaf);
}

void Plant::Mutate()
{
    srand(time(NULL));
    int rng = rand() % (m_DNA.GrowthChoice.size() + m_DNA.LeafGrowAction.size() + m_DNA.RootGrowAction.size());
    switch (rng % 3)
    {
    case 1:
        m_DNA.GrowthChoice[rng % m_DNA.GrowthChoice.size()] = rand() % m_DNA.MAX_VALUE;
        return;
    case 2:
        m_DNA.LeafGrowAction[rng % m_DNA.LeafGrowAction.size()] = rand() % m_DNA.MAX_VALUE;
        return;
    case 3:
        m_DNA.RootGrowAction[rng % m_DNA.RootGrowAction.size()] = rand() % m_DNA.MAX_VALUE;
        return;
    }
}

void Plant::MineSoil(glm::uvec3 pos)
{
    glm::uvec3 area(3, 3, 3);
    for (int x = -area.x / 2; x < area.x / 2; x++)
        for (int y = -area.y / 2; y < area.y / 2; y++)
            for (int z = -area.z / 2; z < area.z / 2; z++)
            {
                glm::uvec3 p(x + pos.x, y + pos.x, z + pos.x);

                switch (m_World->GetVoxel(p))
                {
                case VoxelTypeWater:
                {
                    int max = WATER_MINE_MPLR * GetWaterBonus();
                    int mine = max <= GetRemainingWaterCapacity() ? max : GetRemainingWaterCapacity();
                    m_Water += mine;
                    return;
                }
                case VoxelTypeSoil:
                {
                    SoilResources capacity = GetRemainingSoilResourcesCapacity();
                    SoilResources max = {
                        .Potassium = SOIL_MINE_MPLR * GetSoilBonus(),
                        .Phosphorus = SOIL_MINE_MPLR * GetSoilBonus(),
                        .Nitrogen = SOIL_MINE_MPLR * GetSoilBonus(),
                    };
                    SoilResources toMine = {
                        .Potassium = max.Potassium <= capacity.Potassium ? max.Potassium : capacity.Potassium,
                        .Phosphorus = max.Phosphorus <= capacity.Phosphorus ? max.Phosphorus : capacity.Phosphorus,
                        .Nitrogen = max.Nitrogen <= capacity.Nitrogen ? max.Nitrogen : capacity.Nitrogen,
                    };
                    SoilResources mined = m_World->MineSoilResources(p, toMine);
                    m_SoilResources.Potassium += mined.Potassium;
                    m_SoilResources.Phosphorus += mined.Phosphorus;
                    m_SoilResources.Nitrogen += mined.Nitrogen;
                    return;
                }
                }
            }
}
