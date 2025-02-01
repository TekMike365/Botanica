#include "SimulationLayer.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&SimulationLayer::x, this, std::placeholders::_1)

uint32_t PCGHash(uint32_t seed)
{
    uint32_t state = seed * 747796405u + 2891336453u;
    uint32_t word = ((state >> ((state >> 28u) + 4u)) ^ state) * 277803737u;
    return (word >> 22u) ^ word;
}

SimulationLayer::SimulationLayer(std::shared_ptr<World> world)
    : m_World(world)
{
}

void SimulationLayer::OnAttach()
{
    LogConfig();

    GenerateTerrain();

    // Spawn Plants
    for (int numPlants = SPAWN_PLANT_COUNT; numPlants > 0; numPlants--)
    {
        int x = PCGHash(101 * numPlants + time(NULL)) % m_World->GetSize().x;
        int z = PCGHash(103 * numPlants + time(NULL)) % m_World->GetSize().z;
        m_World->SetVoxel(glm::uvec3(x, m_World->GetSize().y - 1, z), VoxelTypeFruit);
        PlantAPlant(glm::uvec2(x, z));
    }

    Log::SimTrace("Starting Population: {}", m_Plants.size());

    Log::Warn("Paused (F3): {}", m_Paused);
}

void SimulationLayer::OnUpdate(Timestep dt)
{
    if (m_Paused && !m_Step)
        return;

    if (!m_Step)
        if (m_Timer < 1.0f / TPS)
        {
            m_Timer += dt.GetSeconds();
            return;
        }
    m_Timer = 0;
    m_Step = false;
    m_TickCounter++;
    Log::SimTrace("Tick!");

    // Simulation loop
    m_World->ResetResources();
    for (auto plant = m_Plants.begin(); plant != m_Plants.end();)
    {
        plant->Mine();
        plant->Grow();
        if (!plant->IsAlive())
        {
            plant->Die();
            m_Plants.erase(plant);

            if (m_Plants.size() == 0)
            {
                m_Paused = true;
                Log::Warn("Paused (F3): {}", m_Paused);
            }

            continue;
        }
        plant->Survive();
        plant++;
    }

    Log::SimTrace("({}) Population: {}", m_TickCounter, m_Plants.size());
}

void SimulationLayer::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_CALLBACK(OnKeyReleased));
}

bool SimulationLayer::OnKeyReleased(KeyReleasedEvent &e)
{
    switch (e.GetKey())
    {
    case GLFW_KEY_F3:
        m_Paused = !m_Paused;
        Log::Warn("Paused (F3): {}", m_Paused);
        return true;
    case GLFW_KEY_F4:
        m_Step = true;
        return true;
    }

    return false;
}

void SimulationLayer::PlantAPlant(glm::uvec2 xzPos)
{
    uint32_t y = m_World->GetSize().y;
    for (; y > 0, y--;)
    {
        glm::uvec3 pos(xzPos.x, y, xzPos.y);
        if (m_World->GetVoxel(pos) != VoxelTypeSoil)
            continue;

        m_Plants.emplace_back(m_NextID++, m_World, pos);
        auto plant = --m_Plants.end();
        if (!plant->IsAlive())
        {
            plant->Die();
            m_Plants.erase(plant);
        }

        return;
    }
}

void SimulationLayer::GenerateTerrain()
{
    m_WorldSeed = time(NULL);
    // m_WorldSeed = 1736689503; // m_WaterLevel = 6
    // m_WorldSeed = 1736689759; // m_WaterLevel = 6
    Log::Info("Seed: {}", m_WorldSeed);
    srand(m_WorldSeed);

    std::vector<int> perlinNoiseRandomVector;

    for (int i = 0; i < 4; i++)
    {
        perlinNoiseRandomVector.emplace_back(rand() % 16);
    }

    const float PI = 3.141592f;

    for (int x = 0; x < m_World->GetSize().x; x++)
    {
        for (int z = 0; z < m_World->GetSize().z; z++)
        {
            float height = 0.0f;

            glm::vec2 corners[] = {
                glm::vec2(x + 0.5f, z + 0.5f),
                glm::vec2(m_World->GetSize().x - x - 0.5f, z + 0.5f),
                glm::vec2(x + 0.5f, m_World->GetSize().z - z - 0.5f),
                glm::vec2(m_World->GetSize().x - x - 0.5f, m_World->GetSize().z - z - 0.5f)};

            for (int i = 0; i < 4; i++)
            {
                glm::vec2 corner = glm::normalize(corners[i]);

                float randomAngle = perlinNoiseRandomVector[i] / 16.0f * 2.0f * PI;
                glm::vec2 randAngle(cos(randomAngle), sin(randomAngle));

                float dotProduct = glm::dot(corner, randAngle);

                float normalizedHeight = ((dotProduct + 1.0f) / 2.0f) * HEIGHT_SCALAR;

                height += normalizedHeight;
            }

            int finalHeight = (int)(height / 4.0f);
            for (int y = finalHeight; y < WATER_LEVEL; y++)
                m_World->SetVoxel(glm::uvec3(x, y, z), VoxelTypeWater);
            for (int y = 0; y < finalHeight; y++)
                m_World->SetVoxel(glm::uvec3(x, y, z), VoxelTypeSoil);
        }
    }
}

void SimulationLayer::LogConfig() const
{
    Log::SimTrace("SimulationConfig:");
    Log::SimTrace("    Simulation:");
    Log::SimTrace("        WorldSeed: {}", m_WorldSeed);
    Log::SimTrace("        SPAWN_PLANT_COUNT: {}", SPAWN_PLANT_COUNT);
    Log::SimTrace("        HEIGHT_SCALAR: {}", HEIGHT_SCALAR);
    Log::SimTrace("        WATER_LEVEL: {}", WATER_LEVEL);
    Log::SimTrace("    PlantDNA:");
    Log::SimTrace("        MAX_VALUE: {}", PlantDNA::MAX_VALUE);
    Log::SimTrace("        MIN_GROW_ACTION_VALUE: {}", PlantDNA::MIN_GROW_ACTION_VALUE);
    Log::SimTrace("        MIN_ROOT_GROW_CHOICE_VAL: {}", PlantDNA::MIN_ROOT_GROW_CHOICE_VAL);
    Log::SimTrace("        MIN_STEM_GROW_CHOICE_VAL: {}", PlantDNA::MIN_STEM_GROW_CHOICE_VAL);
    Log::SimTrace("        MIN_LEAF_GROW_CHOICE_VAL: {}", PlantDNA::MIN_LEAF_GROW_CHOICE_VAL);
    Log::SimTrace("        MIN_FRUIT_GROW_CHOICE_VAL: {}", PlantDNA::MIN_FRUIT_GROW_CHOICE_VAL);
    Log::SimTrace("    Plant:");
    Log::SimTrace("        SOIL_STORAGE_MPLR:");
    Log::SimTrace("            Potassium: {}", Plant::SOIL_STORAGE_MPLR.Potassium);
    Log::SimTrace("            Phosphorus: {}", Plant::SOIL_STORAGE_MPLR.Phosphorus);
    Log::SimTrace("            Nitrogen: {}", Plant::SOIL_STORAGE_MPLR.Nitrogen);
    Log::SimTrace("        WATER_STORAGE_MPLR: {}", Plant::WATER_STORAGE_MPLR);
    Log::SimTrace("        LIGHT_STORAGE_MPLR: {}", Plant::LIGHT_STORAGE_MPLR);
    Log::SimTrace("        WATER_MINE_MPLR: {}", Plant::WATER_MINE_MPLR);
    Log::SimTrace("        LIGHT_MINE_MPLR: {}", Plant::LIGHT_MINE_MPLR);
    Log::SimTrace("        SOIL_MINE_MPLR: {}", Plant::SOIL_MINE_MPLR);
    Log::SimTrace("        SOIL_WATER_MPLR: {}", Plant::SOIL_WATER_MPLR);
    Log::SimTrace("        POTASSIUM_BONUS_CONSTANT: {}", Plant::POTASSIUM_BONUS_CONSTANT);
    Log::SimTrace("        PHOSPHORUS_BONUS_CONSTANT: {}", Plant::PHOSPHORUS_BONUS_CONSTANT);
    Log::SimTrace("        NITROGEN_BONUS_CONSTANT: {}", Plant::NITROGEN_BONUS_CONSTANT);
    Log::SimTrace("        PLANT_PART_WATER_COST: {}", Plant::PLANT_PART_WATER_COST);
    Log::SimTrace("        PLANT_PART_LIGHT_COST: {}", Plant::PLANT_PART_LIGHT_COST);
    Log::SimTrace("        WATER_SURVIVE_COST_MPLR: {}", Plant::WATER_SURVIVE_COST_MPLR);
    Log::SimTrace("        LIGHT_SURVIVE_COST_MPLR: {}", Plant::LIGHT_SURVIVE_COST_MPLR);
    Log::SimTrace("        POTASSIUM_SURVIVE_COST_MPLR: {}", Plant::POTASSIUM_SURVIVE_COST_MPLR);
    Log::SimTrace("        PHOSPHORUS_SURVIVE_COST_MPLR: {}", Plant::PHOSPHORUS_SURVIVE_COST_MPLR);
    Log::SimTrace("        NITROGEN_SURVIVE_COST_MPLR: {}", Plant::NITROGEN_SURVIVE_COST_MPLR);
    Log::SimTrace("        START_RESOURCES_MPLR: {}", Plant::START_RESOURCES_MPLR);
    Log::SimTrace("        ROOT_POTASSIUM_COST: {}", Plant::ROOT_POTASSIUM_COST);
    Log::SimTrace("        ROOT_NITROGEN_COST: {}", Plant::ROOT_NITROGEN_COST);
    Log::SimTrace("        LEAF_PHOSPHORUS_COST: {}", Plant::LEAF_PHOSPHORUS_COST);
    Log::SimTrace("        STEM_PHOSPHORUS_COST: {}", Plant::STEM_PHOSPHORUS_COST);
    Log::SimTrace("        STEM_POTASSIUM_COST: {}", Plant::STEM_POTASSIUM_COST);
    Log::SimTrace("        STEM_NITROGEN_COST: {}", Plant::STEM_NITROGEN_COST);
    Log::SimTrace("        FRUIT_PHOSPHORUS_COST: {}", Plant::FRUIT_PHOSPHORUS_COST);
    Log::SimTrace("        FRUIT_POTASSIUM_COST: {}", Plant::FRUIT_POTASSIUM_COST);
    Log::SimTrace("        FRUIT_NITROGEN_COST: {}", Plant::FRUIT_NITROGEN_COST);
}
