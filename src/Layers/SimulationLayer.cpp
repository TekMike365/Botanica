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
    PlantAPlant(glm::uvec2(m_World->GetSize().x / 2 - 3, m_World->GetSize().z / 2));

    Log::Warn("Paused (F3): {}", m_Paused);
}

void SimulationLayer::OnUpdate(Timestep dt)
{
    if (m_Paused)
        return;

    if (m_Timer < 1.0f / TPS)
    {
        m_Timer += dt.GetSeconds();
        return;
    }
    m_Timer = 0;
    BT_DLOG_TRACE("Tick!");

    // Simulation loop
    m_World->ResetResources();
    for (auto plant = m_Plants.begin(); plant != m_Plants.end();)
    {
        plant->Mine();
        plant->Grow();
        if (!plant->IsAlive())
        {
            BT_DLOG_WARN("Plant died.");
            m_Plants.erase(plant);
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

        m_Plants.emplace_back(m_World, pos);
        if (!(--m_Plants.end())->IsAlive())
            m_Plants.erase(--m_Plants.end());

        return;
    }
}

void SimulationLayer::GenerateTerrain()
{
    // uint32_t seed = time(NULL);
    // uint32_t seed = 1736689503; // m_WaterLevel = 6
    uint32_t seed = 1736689759; // m_WaterLevel = 6
    Log::Info("Seed: {}", seed);
    srand(seed);

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
