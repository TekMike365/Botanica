#include "SimulationLayer.h"

SimulationLayer::SimulationLayer(World &world)
    :m_World(world)
{
}

void SimulationLayer::OnAttach()
{
    GenerateTerrain();
}

void SimulationLayer::OnUpdate(Timestep dt)
{
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

    for (int x = 0; x < m_World.GetSize().x; x++)
    {
        for (int z = 0; z < m_World.GetSize().z; z++)
        {
            float height = 0.0f;

            glm::vec2 corners[] = {
                glm::vec2(x + 0.5f, z + 0.5f),
                glm::vec2(m_World.GetSize().x - x - 0.5f, z + 0.5f),
                glm::vec2(x + 0.5f, m_World.GetSize().z - z - 0.5f),
                glm::vec2(m_World.GetSize().x - x - 0.5f, m_World.GetSize().z - z - 0.5f)};

            for (int i = 0; i < 4; i++)
            {
                glm::vec2 corner = glm::normalize(corners[i]);

                float randomAngle = perlinNoiseRandomVector[i] / 16.0f * 2.0f * PI;
                glm::vec2 randAngle(cos(randomAngle), sin(randomAngle));

                float dotProduct = glm::dot(corner, randAngle);

                float normalizedHeight = ((dotProduct + 1.0f) / 2.0f) * m_HeightScalar;

                height += normalizedHeight;
            }

            int finalHeight = (int)(height / 4.0f);
            for (int y = finalHeight; y < m_WaterLevel; y++)
                m_World.SetVoxel(glm::uvec3(x, y, z), VoxelTypeWater);
            for (int y = 0; y < finalHeight; y++)
                m_World.SetVoxel(glm::uvec3(x, y, z), VoxelTypeSoil);
        }
    }
}
