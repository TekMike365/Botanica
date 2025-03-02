#include "pch.h"
#include "SimulationLayer.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&SimulationLayer::x, this, std::placeholders::_1)

SimulationLayer::SimulationLayer(std::shared_ptr<World> world)
    : m_World(world)
{
}

void SimulationLayer::OnAttach()
{
    GenerateTerrain();

    // Spawn Plants
    for (int numPlants = SPAWN_PLANT_COUNT; numPlants > 0; numPlants--)
    {
        int x = PCGHash(101 * numPlants + time(NULL)) % m_World->GetSize().x;
        int z = PCGHash(103 * numPlants + time(NULL)) % m_World->GetSize().z;
        // m_World->SetVoxel(glm::uvec3(x, m_World->GetSize().y - 1, z), VoxelTypeFruit);
        PlantAPlant(glm::uvec2(x, z));
    }

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

    // Simulation loop
    m_World->ResetResources();

    for (int i = 0; i < m_Plants.size(); i++)
    {
        auto plant = m_Plants.begin() + i;

        auto plants = plant->Reproduce(m_NextID);
        m_Plants.insert(m_Plants.end(), plants.begin(), plants.end());
        // In case the iterator changes
        plant = m_Plants.begin() + i;
    }

    for (auto plant = m_Plants.begin(); plant != m_Plants.end();)
    {
        plant->Mine();
        plant->Grow();
        if (!plant->IsAlive())
        {
            plant->Die();
            plant = m_Plants.erase(plant);

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