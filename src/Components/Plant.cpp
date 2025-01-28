#include "pch.h"
#include "Plant.h"

Plant::Plant(std::shared_ptr<World> world, glm::uvec3 pos)
    : m_World(world), m_Pos(pos)
{
    srand(time(NULL));

    std::array<int, 4> growthChoice;
    for (int &i : growthChoice)
        i = rand() % m_DNA.MAX_VALUE;

    std::array<int, 27> leafGrowAction;
    for (int &i : leafGrowAction)
        i = rand() % m_DNA.MAX_VALUE;

    std::array<int, 27> rootGrowAction;
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
    for (auto p : m_RootPositions)
        MineSoil(p);
    for (auto p : m_LeafPositions)
        MineAir(p);
}

void Plant::Reproduce()
{
}

void Plant::Grow()
{
    int rng = rand() % PlantDNA::MAX_VALUE;

    if (rng < m_DNA.GrowthChoice[0])
        GrowRoot();
    else if (rng < m_DNA.GrowthChoice[1])
        GrowStem();
    else if (rng < m_DNA.GrowthChoice[2])
        GrowLeaf();
    else if (rng < m_DNA.GrowthChoice[3])
        GrowFruit();
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
                glm::uvec3 voxPos(x + pos.x, y + pos.x, z + pos.x);

                switch (m_World->GetVoxel(voxPos))
                {
                case VoxelTypeWater:
                {
                    int max = WATER_MINE_MPLR * GetWaterBonus();
                    int mine = max <= GetRemainingWaterCapacity() ? max : GetRemainingWaterCapacity();
                    m_Water += mine;
                    continue;
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
                    SoilResources mined = m_World->MineSoilResources(voxPos, toMine);
                    m_SoilResources.Potassium += mined.Potassium;
                    m_SoilResources.Phosphorus += mined.Phosphorus;
                    m_SoilResources.Nitrogen += mined.Nitrogen;
                    continue;
                }
                }
            }
}

void Plant::MineAir(glm::uvec3 pos)
{
    glm::uvec3 area(3, 3, 3);
    for (int x = -area.x / 2; x < area.x / 2; x++)
        for (int y = 1; y < area.y + 1; y++)
            for (int z = -area.z / 2; z < area.z / 2; z++)
            {
                glm::uvec3 voxPos(x + pos.x, y + pos.x, z + pos.x);
                switch (m_World->GetVoxel(voxPos))
                {
                case VoxelTypeAir:
                {
                    int max = LIGHT_MINE_MPLR * GetLightBonus();
                    int mine = max <= GetRemainingLightCapacity() ? max : GetRemainingLightCapacity();
                    m_Light += mine;
                    continue;
                }
                }
            }
}

bool Plant::Seed(glm::uvec2 xzPos)
{
    return false;
}

void Plant::GrowRoot()
{
    for (auto p : m_RootPositions)
    {
        int rng = rand() % PlantDNA::MAX_VALUE;
        for (int i = 0; i < m_DNA.RootGrowAction.size(); i++)
        {
            int gene = m_DNA.RootGrowAction[i];

            uint32_t x = i / (3 * 3);
            uint32_t y = (i / 3) % 3;
            uint32_t z = i % 3;
            glm::uvec3 voxPos(x + p.x, y + p.y, z + p.z);

            if (m_World->GetVoxel(voxPos) != VoxelTypeSoil || rng > gene)
                continue;

            // Check resources
            const int nitrogenCost = 10;
            const int potassiumCost = 10;
            if (m_Water < PLANT_PART_WATER_COST ||
                m_Light < PLANT_PART_LIGHT_COST ||
                m_SoilResources.Nitrogen < nitrogenCost ||
                m_SoilResources.Potassium < potassiumCost)

                continue;

            // pay for it
            m_Water -= PLANT_PART_WATER_COST;
            m_Light -= PLANT_PART_LIGHT_COST;
            m_SoilResources.Nitrogen -= nitrogenCost;
            m_SoilResources.Potassium -= potassiumCost;

            // Grow
            m_RootPositions.emplace_back(voxPos);
            m_World->SetVoxel(voxPos, VoxelTypeRoot);
            return;
        }
    }
}

void Plant::GrowLeaf()
{
    for (auto p : m_LeafPositions)
    {
        int rng = rand() % PlantDNA::MAX_VALUE;
        for (int i = 0; i < m_DNA.LeafGrowAction.size(); i++)
        {
            int gene = m_DNA.LeafGrowAction[i];

            uint32_t x = i / (3 * 3);
            uint32_t y = (i / 3) % 3;
            uint32_t z = i % 3;
            glm::uvec3 voxPos(x + p.x, y + p.y, z + p.z);

            if (m_World->GetVoxel(voxPos) != VoxelTypeAir || rng > gene)
                continue;

            // Check resources
            const int phosphorusCost = 10;
            if (m_Water < PLANT_PART_WATER_COST ||
                m_Light < PLANT_PART_LIGHT_COST ||
                m_SoilResources.Phosphorus < phosphorusCost)

                continue;

            // pay for it
            m_Water -= PLANT_PART_WATER_COST;
            m_Light -= PLANT_PART_LIGHT_COST;
            m_SoilResources.Phosphorus -= phosphorusCost;

            // Grow
            m_LeafPositions.emplace_back(voxPos);
            m_World->SetVoxel(voxPos, VoxelTypeLeaf);
            return;
        }
    }
}

void Plant::GrowStem()
{
    // Check resources
    const int phosphorusCost = 5;
    const int nitrogenCost = 5;
    const int potassiumCost = 5;
    if (m_Water < PLANT_PART_WATER_COST ||
        m_Light < PLANT_PART_LIGHT_COST ||
        m_SoilResources.Phosphorus < phosphorusCost ||
        m_SoilResources.Nitrogen < nitrogenCost ||
        m_SoilResources.Potassium < potassiumCost)

        return;

    // pay for it
    m_Water -= PLANT_PART_WATER_COST;
    m_Light -= PLANT_PART_LIGHT_COST;
    m_SoilResources.Phosphorus -= phosphorusCost;
    m_SoilResources.Nitrogen -= nitrogenCost;
    m_SoilResources.Potassium -= potassiumCost;

    for (auto &p : m_LeafPositions)
        p.y += 1;

    glm::uvec3 pos = m_StemPositions.back();
    pos.y += 1;

    m_StemPositions.emplace_back(pos);
    m_World->SetVoxel(pos, VoxelTypeStem);
}

void Plant::GrowFruit()
{
    int i = rand() % m_LeafPositions.size();
    auto it = m_LeafPositions.begin() + i;
    // Check resources
    const int phosphorusCost = 7;
    const int nitrogenCost = 7;
    const int potassiumCost = 7;
    if (m_Water < PLANT_PART_WATER_COST ||
        m_Light < PLANT_PART_LIGHT_COST ||
        m_SoilResources.Phosphorus < phosphorusCost ||
        m_SoilResources.Nitrogen < nitrogenCost ||
        m_SoilResources.Potassium < potassiumCost)

        return;

    // pay for it
    m_Water -= PLANT_PART_WATER_COST;
    m_Light -= PLANT_PART_LIGHT_COST;
    m_SoilResources.Phosphorus -= phosphorusCost;
    m_SoilResources.Nitrogen -= nitrogenCost;
    m_SoilResources.Potassium -= potassiumCost;

    // Grow
    glm::uvec3 voxPos = *it;
    m_LeafPositions.erase(it);
    m_FruitPositions.emplace_back(voxPos);
    m_World->SetVoxel(voxPos, VoxelTypeFruit);
}
