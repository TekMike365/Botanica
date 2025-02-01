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

template <typename Iter>
int WeightedChoice(Iter begin, Iter end)
{
    int sum = 0;
    for (Iter it = begin; it != end; it++)
        sum += *it;

    int rng = (int)(PCGHash(211 * time(NULL)) % sum);

    int idx = 0;
    for (Iter it = begin; rng > *it && it != end; it++, idx++)
        rng -= *it;

    return idx;
}

Plant::Plant(int id, std::shared_ptr<World> world, glm::uvec3 pos)
    : m_ID(id), m_World(world), m_Pos(pos)
{
    srand(time(NULL));

    std::array<int, m_DNA.GROWTH_CHOICE_LEN> growthChoice{
        (int)(PCGHash(101 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_ROOT_GROW_CHOICE_VAL)) + m_DNA.MIN_ROOT_GROW_CHOICE_VAL,
        (int)(PCGHash(103 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_STEM_GROW_CHOICE_VAL)) + m_DNA.MIN_STEM_GROW_CHOICE_VAL,
        (int)(PCGHash(107 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_LEAF_GROW_CHOICE_VAL)) + m_DNA.MIN_LEAF_GROW_CHOICE_VAL,
        (int)(PCGHash(109 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_FRUIT_GROW_CHOICE_VAL)) + m_DNA.MIN_FRUIT_GROW_CHOICE_VAL,
    };

    std::array<int, m_DNA.LEAF_GROW_ACTION_LEN> leafGrowAction;
    for (int &i : leafGrowAction)
        i = (int)(PCGHash(i + 113 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_GROW_ACTION_VALUE)) + m_DNA.MIN_GROW_ACTION_VALUE;

    std::array<int, m_DNA.ROOT_GROW_ACTION_LEN> rootGrowAction;
    for (int &i : rootGrowAction)
        i = int(PCGHash(i + 127 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_GROW_ACTION_VALUE)) + m_DNA.MIN_GROW_ACTION_VALUE;

    m_DNA = {
        .GrowthChoice = growthChoice,
        .RootGrowAction = rootGrowAction,
        .LeafGrowAction = leafGrowAction,
    };

    Log::SimInfo("[pid: {}] Created new plant.", m_ID);
    Log::SimInfo("[pid: {}]     Position: [{}, {}, {}]", m_ID, m_Pos.x, m_Pos.y, m_Pos.z);
    LogDNA();

    Init();
}

Plant::Plant(int id, std::shared_ptr<World> world, glm::uvec3 pos, const PlantDNA &dna)
    : m_ID(id), m_World(world), m_Pos(pos), m_DNA(dna)
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

std::vector<Plant> Plant::Reproduce(int &nextID)
{
    std::vector<Plant> newPlants;
    for (auto it = m_FruitPositions.begin(); it != m_FruitPositions.end();)
    {
        bool inc = 1;
        for (int tries = 10; tries > 0; tries--)
        {
            glm::uvec2 xzPos(
                PCGHash(101 * m_ID * time(NULL)) % m_World->GetSize().x,
                PCGHash(103 * m_ID * time(NULL)) % m_World->GetSize().z);

            Plant plant = Seed(xzPos, nextID++);
            if (!plant.IsAlive())
                continue;

            m_World->SetVoxel(*it, VoxelTypeLeaf);
            m_LeafPositions.emplace_back(*it);
            m_FruitPositions.erase(it);
            newPlants.emplace_back(plant);
            inc = 0;
            break;
        }
        it += inc;
    }

    if (newPlants.size() > 0)
    {
        Log::SimInfo("[pid: {}] Plant has reproduced.", m_ID);
        LogPosVector(m_FruitPositions, "Plant Fruit positions");
        LogPosVector(m_LeafPositions, "Plant Leaf positions");
    }

    return newPlants;
}

void Plant::Grow()
{
    int idx = WeightedChoice<std::array<int, m_DNA.GROWTH_CHOICE_LEN>::const_iterator>(m_DNA.GrowthChoice.begin(), m_DNA.GrowthChoice.end());
    switch (idx)
    {
    case 0:
        GrowRoot();
        break;
    case 1:
        GrowStem();
        break;
    case 2:
        GrowLeaf();
        break;
    case 3:
        GrowFruit();
        break;
    }
}

void Plant::Survive()
{
    m_Water -= WATER_SURVIVE_COST_MPLR * GetSize();
    m_Light -= LIGHT_SURVIVE_COST_MPLR * GetSize();
    m_SoilResources.Nitrogen -= NITROGEN_SURVIVE_COST_MPLR * GetSize();
    m_SoilResources.Phosphorus -= PHOSPHORUS_SURVIVE_COST_MPLR * GetSize();
    m_SoilResources.Potassium -= POTASSIUM_SURVIVE_COST_MPLR * GetSize();
    LogPlantResources();
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
    Log::SimWarn("[pid: {}] Plant died.", m_ID);
}

bool Plant::IsAlive() const
{
    if (m_RootPositions.size() == 0 || m_LeafPositions.size() == 0 || m_StemPositions.size() == 0)
        return false;

    if (m_Water < WATER_SURVIVE_COST_MPLR * GetSize())
    {
        Log::SimInfo("[pid: {}] Plant ain't got enough Water: {}/{}", m_ID, m_Water, WATER_SURVIVE_COST_MPLR * GetSize());
        return false;
    }

    if (m_Light < LIGHT_SURVIVE_COST_MPLR * GetSize())
    {
        Log::SimInfo("[pid: {}] Plant ain't got enough Light: {}/{}", m_ID, m_Light, LIGHT_SURVIVE_COST_MPLR * GetSize());
        return false;
    }

    if (m_SoilResources.Nitrogen < NITROGEN_SURVIVE_COST_MPLR * GetSize())
    {
        Log::SimInfo("[pid: {}] Plant ain't got enough Nitrogen: {}/{}", m_ID, m_SoilResources.Nitrogen, NITROGEN_SURVIVE_COST_MPLR * GetSize());
        return false;
    }

    if (m_SoilResources.Phosphorus < PHOSPHORUS_SURVIVE_COST_MPLR * GetSize())
    {
        Log::SimInfo("[pid: {}] Plant ain't got enough Phosphorus: {}/{}", m_ID, m_SoilResources.Phosphorus, PHOSPHORUS_SURVIVE_COST_MPLR * GetSize());
        return false;
    }

    if (m_SoilResources.Potassium < POTASSIUM_SURVIVE_COST_MPLR * GetSize())
    {
        Log::SimInfo("[pid: {}] Plant ain't got enough Potassium: {}/{}", m_ID, m_SoilResources.Potassium, POTASSIUM_SURVIVE_COST_MPLR * GetSize());
        return false;
    }

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
        Log::SimWarn("[pid: {}] Plant wasn't able to spawn.", m_ID);
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

    LogPlantResources();
}

void Plant::Mutate()
{
    srand(time(NULL));
    int rng = (int)(PCGHash(227 * m_ID * time(NULL)) % (m_DNA.GrowthChoice.size() + m_DNA.LeafGrowAction.size() + m_DNA.RootGrowAction.size()));
    switch (rng % 3)
    {
    case 1:
        switch (rng % m_DNA.GrowthChoice.size())
        {
        case 0:
            m_DNA.GrowthChoice[0] = (int)(PCGHash(101 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_ROOT_GROW_CHOICE_VAL)) + m_DNA.MIN_ROOT_GROW_CHOICE_VAL;
            Log::SimInfo("[pid: {}] Plant Mutated.", m_ID);
            LogDNA();
            return;
        case 1:
            m_DNA.GrowthChoice[1] = (int)(PCGHash(103 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_STEM_GROW_CHOICE_VAL)) + m_DNA.MIN_STEM_GROW_CHOICE_VAL;
            Log::SimInfo("[pid: {}] Plant Mutated.", m_ID);
            LogDNA();
            return;
        case 2:
            m_DNA.GrowthChoice[2] = (int)(PCGHash(107 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_LEAF_GROW_CHOICE_VAL)) + m_DNA.MIN_LEAF_GROW_CHOICE_VAL;
            Log::SimInfo("[pid: {}] Plant Mutated.", m_ID);
            LogDNA();
            return;
        case 3:
            m_DNA.GrowthChoice[3] = (int)(PCGHash(109 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_FRUIT_GROW_CHOICE_VAL)) + m_DNA.MIN_FRUIT_GROW_CHOICE_VAL;
            Log::SimInfo("[pid: {}] Plant Mutated.", m_ID);
            LogDNA();
            return;
        }
    case 2:
        m_DNA.LeafGrowAction[rng % m_DNA.LeafGrowAction.size()] = (int)(PCGHash(113 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_GROW_ACTION_VALUE)) + m_DNA.MIN_GROW_ACTION_VALUE;
        Log::SimInfo("[pid: {}] Plant Mutated.", m_ID);
        LogDNA();
        return;
    case 3:
        m_DNA.RootGrowAction[rng % m_DNA.RootGrowAction.size()] = (int)(PCGHash(127 * m_ID * time(NULL)) % (m_DNA.MAX_VALUE - m_DNA.MIN_GROW_ACTION_VALUE)) + m_DNA.MIN_GROW_ACTION_VALUE;
        Log::SimInfo("[pid: {}] Plant Mutated.", m_ID);
        LogDNA();
        return;
    }
}

void Plant::MineSoil(glm::uvec3 pos)
{
    glm::uvec3 area(3, 3, 3);
    glm::ivec3 offset(-1, -1, -1);
    for (int x = 0; x < area.x; x++)
        for (int y = 0; y < area.y; y++)
            for (int z = 0; z < area.z; z++)
            {
                glm::uvec3 voxPos(
                    Clamp(x + pos.x + offset.x, 0, m_World->GetSize().x),
                    Clamp(y + pos.y + offset.y, 0, m_World->GetSize().y),
                    Clamp(z + pos.z + offset.z, 0, m_World->GetSize().z));

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
                    {
                        int max = SOIL_WATER_MPLR * GetWaterBonus();
                        int mine = max <= GetRemainingWaterCapacity() ? max : GetRemainingWaterCapacity();
                        m_Water += mine;
                    }

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

    LogPlantResources();
}

void Plant::MineAir(glm::uvec3 pos)
{
    glm::uvec3 area(3, 1, 3);
    glm::ivec3 offset(-1, 1, -1);
    for (int x = 0; x < area.x; x++)
        for (int y = 0; y < area.y; y++)
            for (int z = 0; z < area.z; z++)
            {
                glm::uvec3 voxPos(
                    Clamp(x + pos.x + offset.x, 0, m_World->GetSize().x),
                    Clamp(y + pos.y + offset.y, 0, m_World->GetSize().y),
                    Clamp(z + pos.z + offset.z, 0, m_World->GetSize().z));

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

    LogPlantResources();
}

Plant Plant::Seed(glm::uvec2 xzPos, int id)
{
    uint32_t y = m_World->GetSize().y;
    for (; y > 0, y--;)
    {
        glm::uvec3 pos(xzPos.x, y, xzPos.y);
        if (m_World->GetVoxel(pos) != VoxelTypeSoil)
            continue;

        return Plant(id, m_World, pos);
    }

    return Plant(id, m_World, glm::uvec3((uint32_t)-1, (uint32_t)-1, (uint32_t)-1));
}

void Plant::GrowRoot()
{
    for (auto p : m_RootPositions)
    {
        int idx = WeightedChoice<std::array<int, m_DNA.ROOT_GROW_ACTION_LEN>::const_iterator>(m_DNA.RootGrowAction.begin(), m_DNA.RootGrowAction.end());

        int x = Clamp(idx / (3 * 3) - 1 + p.x, 0, m_World->GetSize().x);
        int y = Clamp((idx / 3) % 3 - 1 + p.y, 0, m_World->GetSize().x);
        int z = Clamp(idx % 3 - 1 + p.z, 0, m_World->GetSize().x);
        glm::uvec3 voxPos(x, y, z);

        if (m_World->GetVoxel(voxPos) != VoxelTypeSoil)
            continue;

        // Check resources
        if (m_Water < PLANT_PART_WATER_COST ||
            m_Light < PLANT_PART_LIGHT_COST ||
            m_SoilResources.Nitrogen < ROOT_NITROGEN_COST ||
            m_SoilResources.Potassium < ROOT_POTASSIUM_COST)

            continue; // Ain't enough

        // pay for it
        m_Water -= PLANT_PART_WATER_COST;
        m_Light -= PLANT_PART_LIGHT_COST;
        m_SoilResources.Nitrogen -= ROOT_NITROGEN_COST;
        m_SoilResources.Potassium -= ROOT_POTASSIUM_COST;

        // Grow
        m_RootPositions.emplace_back(voxPos);
        m_World->SetVoxel(voxPos, VoxelTypeRoot);

        Log::SimInfo("[pid: {}] Plant has grown.", m_ID);
        LogPosVector(m_RootPositions, "Plant Root positions");
        LogPlantResources();
        return;
    }
}

void Plant::GrowLeaf()
{
    for (auto p : m_LeafPositions)
    {
        int idx = WeightedChoice<std::array<int, m_DNA.LEAF_GROW_ACTION_LEN>::const_iterator>(m_DNA.LeafGrowAction.begin(), m_DNA.LeafGrowAction.end());

        int x = Clamp(idx / (3 * 3) - 1 + p.x, 0, m_World->GetSize().x);
        int y = Clamp((idx / 3) % 3 - 1 + p.y, 0, m_World->GetSize().x);
        int z = Clamp(idx % 3 - 1 + p.z, 0, m_World->GetSize().x);
        glm::uvec3 voxPos(x, y, z);

        if (m_World->GetVoxel(voxPos) != VoxelTypeAir)
            continue;

        // Check resources
        if (m_Water < PLANT_PART_WATER_COST ||
            m_Light < PLANT_PART_LIGHT_COST ||
            m_SoilResources.Phosphorus < LEAF_PHOSPHORUS_COST)

            continue; // Ain't enough

        // pay for it
        m_Water -= PLANT_PART_WATER_COST;
        m_Light -= PLANT_PART_LIGHT_COST;
        m_SoilResources.Phosphorus -= LEAF_PHOSPHORUS_COST;

        // Grow
        m_LeafPositions.emplace_back(voxPos);
        m_World->SetVoxel(voxPos, VoxelTypeLeaf);

        Log::SimInfo("[pid: {}] Plant has grown.", m_ID);
        LogPosVector(m_LeafPositions, "Plant Leaf positions");

        LogPlantResources();
        return;
    }
}

void Plant::GrowStem()
{
    // Check resources
    if (m_Water < PLANT_PART_WATER_COST ||
        m_Light < PLANT_PART_LIGHT_COST ||
        m_SoilResources.Phosphorus < STEM_PHOSPHORUS_COST ||
        m_SoilResources.Nitrogen < STEM_NITROGEN_COST ||
        m_SoilResources.Potassium < STEM_POTASSIUM_COST)

        return;

    // pay for it
    m_Water -= PLANT_PART_WATER_COST;
    m_Light -= PLANT_PART_LIGHT_COST;
    m_SoilResources.Phosphorus -= STEM_PHOSPHORUS_COST;
    m_SoilResources.Nitrogen -= STEM_NITROGEN_COST;
    m_SoilResources.Potassium -= STEM_POTASSIUM_COST;

    // Move leaves
    for (auto &p : m_LeafPositions)
    {
        m_World->SetVoxel(p, VoxelTypeAir);
        p.y += 1;
        m_World->SetVoxel(p, VoxelTypeLeaf);
    }

    glm::uvec3 pos = m_StemPositions.back();
    pos.y += 1;

    m_StemPositions.emplace_back(pos);
    m_World->SetVoxel(pos, VoxelTypeStem);

    Log::SimInfo("[pid: {}] Plant has grown.", m_ID);
    LogPosVector(m_StemPositions, "Plant Stem positions");

    LogPlantResources();
}

void Plant::GrowFruit()
{
    if (m_LeafPositions.size() == 0)
        return;

    int i = (int)(PCGHash(101 * m_ID * time(NULL)) % m_LeafPositions.size());
    auto pos = m_LeafPositions.begin() + i;
    // Check resources
    if (m_Water < PLANT_PART_WATER_COST ||
        m_Light < PLANT_PART_LIGHT_COST ||
        m_SoilResources.Phosphorus < FRUIT_PHOSPHORUS_COST ||
        m_SoilResources.Nitrogen < FRUIT_NITROGEN_COST ||
        m_SoilResources.Potassium < FRUIT_POTASSIUM_COST)

        return;

    // pay for it
    m_Water -= PLANT_PART_WATER_COST;
    m_Light -= PLANT_PART_LIGHT_COST;
    m_SoilResources.Phosphorus -= FRUIT_PHOSPHORUS_COST;
    m_SoilResources.Nitrogen -= FRUIT_NITROGEN_COST;
    m_SoilResources.Potassium -= FRUIT_POTASSIUM_COST;

    // Grow
    glm::uvec3 voxPos = *pos;
    m_LeafPositions.erase(pos);
    m_FruitPositions.emplace_back(voxPos);
    m_World->SetVoxel(voxPos, VoxelTypeFruit);

    Log::SimInfo("[pid: {}] Plant has grown.", m_ID);
    LogPosVector(m_FruitPositions, "Plant Fruit positions");
    LogPlantResources();
}
