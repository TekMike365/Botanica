#include "pch.h"
#include "Plant.h"

int Clamp(int val, int min, int max)
{
    if (val >= max)
        return max;
    if (val <= min)
        return min;
    return val;
}

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

void Plant::Mine()
{
    for (auto p : m_RootPositions)
        MineSoil(p);
    for (auto p : m_LeafPositions)
        MineAir(p);
}

std::vector<Plant> Plant::Reproduce()
{
    std::vector<Plant> newPlants;
    for (auto it = m_FruitPositions.begin(); it != m_FruitPositions.end();)
    {
        bool inc = 1;
        for (int tries = 10; tries > 0; tries--)
        {
            glm::uvec2 xzPos(
                rand() % m_World->GetSize().x,
                rand() % m_World->GetSize().z);

            Plant plant = Seed(xzPos);
            if (!plant.IsAlive())
                continue;

            m_FruitPositions.erase(it);
            newPlants.emplace_back(plant);
            inc = 0;
            break;
        }
        it += inc;
    }
    return newPlants;
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

void Plant::Survive()
{
    m_Water -= WATER_SURVIVE_COST_MPLR * GetSize();
    m_Light -= LIGHT_SURVIVE_COST_MPLR * GetSize();
    m_SoilResources.Nitrogen -= NITROGEN_SURVIVE_COST_MPLR * GetSize();
    m_SoilResources.Phosphorus -= PHOSPHORUS_SURVIVE_COST_MPLR * GetSize();
    m_SoilResources.Potassium -= POTASSIUM_SURVIVE_COST_MPLR * GetSize();
}

void Plant::Die()
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

bool Plant::IsAlive() const
{
    if (m_RootPositions.size() == 0 || m_LeafPositions.size() == 0 || m_StemPositions.size() == 0)
        return false;

    return m_Water >= WATER_SURVIVE_COST_MPLR * GetSize() &&
           m_Light >= LIGHT_SURVIVE_COST_MPLR * GetSize() &&
           m_SoilResources.Nitrogen >= NITROGEN_SURVIVE_COST_MPLR * GetSize() &&
           m_SoilResources.Phosphorus >= PHOSPHORUS_SURVIVE_COST_MPLR * GetSize() &&
           m_SoilResources.Potassium >= POTASSIUM_SURVIVE_COST_MPLR * GetSize();
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

    // :DDDD
    m_Water = START_RESOURCES_MPLR * GetRemainingWaterCapacity();
    m_Light = START_RESOURCES_MPLR * GetRemainingLightCapacity();
    SoilResources soilCap = GetRemainingSoilResourcesCapacity();
    m_SoilResources = {
        .Potassium = (int)(START_RESOURCES_MPLR * soilCap.Potassium),
        .Phosphorus = (int)(START_RESOURCES_MPLR * soilCap.Phosphorus),
        .Nitrogen = (int)(START_RESOURCES_MPLR * soilCap.Nitrogen),
    };
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
                        .Potassium = (int)(SOIL_MINE_MPLR * GetSoilBonus()),
                        .Phosphorus = (int)(SOIL_MINE_MPLR * GetSoilBonus()),
                        .Nitrogen = (int)(SOIL_MINE_MPLR * GetSoilBonus()),
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

Plant Plant::Seed(glm::uvec2 xzPos)
{
    uint32_t y = m_World->GetSize().y;
    for (; y > 0, y--;)
    {
        glm::uvec3 pos(xzPos.x, y, xzPos.y);
        if (m_World->GetVoxel(pos) != VoxelTypeSoil)
            continue;

        return Plant(m_World, pos);
    }

    return Plant(m_World, glm::uvec3((uint32_t)-1, (uint32_t)-1, (uint32_t)-1));
}

void Plant::GrowRoot()
{
    for (auto p : m_RootPositions)
    {
        int rng = rand() % PlantDNA::MAX_VALUE;
        for (int i = 0; i < m_DNA.RootGrowAction.size(); i++)
        {
            int gene = m_DNA.RootGrowAction[i];

            int x = Clamp(i / (3 * 3) - 1 + p.x, 0, m_World->GetSize().x);
            int y = Clamp((i / 3) % 3 - 1 + p.y, 0, m_World->GetSize().x);
            int z = Clamp(i % 3 - 1 + p.z, 0, m_World->GetSize().x);
            glm::uvec3 voxPos(x, y, z);

            if (m_World->GetVoxel(voxPos) != VoxelTypeSoil || rng > gene)
                continue;

            // Check resources
            const int nitrogenCost = 5;
            const int potassiumCost = 5;
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

            int x = Clamp(i / (3 * 3) - 1 + p.x, 0, m_World->GetSize().x);
            int y = Clamp((i / 3) % 3 - 1 + p.y, 0, m_World->GetSize().x);
            int z = Clamp(i % 3 - 1 + p.z, 0, m_World->GetSize().x);
            glm::uvec3 voxPos(x, y, z);

            if (m_World->GetVoxel(voxPos) != VoxelTypeAir || rng > gene)
                continue;

            // Check resources
            const int phosphorusCost = 5;
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
    auto pos = m_LeafPositions.begin() + i;
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
    glm::uvec3 voxPos = *pos;
    m_LeafPositions.erase(pos);
    m_FruitPositions.emplace_back(voxPos);
    m_World->SetVoxel(voxPos, VoxelTypeFruit);
}
