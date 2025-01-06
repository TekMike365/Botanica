#include "SimulationLayer.h"
#include "Platform.h"
#include <vector>
#include <cmath>
#include <cstdlib>
#include <ctime>
#include <algorithm>
#include <random>

SimulationLayer::SimulationLayer(World &world)
    :m_World(world)
{
}

void SimulationLayer::OnAttach()
{
    auto sizeX = m_World.VoxelIDs.GetSize().x;
    auto sizeY = m_World.VoxelIDs.GetSize().y;
    auto sizeZ = m_World.VoxelIDs.GetSize().z;

    std::vector<std::vector<int>> height(sizeX, std::vector<int>(sizeZ,0));
    srand(time(NULL));
    std::vector<int> perlinNoiseRandomVector;

    for (int i = 0; i < 4; i++)
    {
        perlinNoiseRandomVector.emplace_back(rand()%16);
    }
    double Pie = 3.14159265358979323846;

    for (int x = 0; x < sizeX; x++) {
        for (int z = 0; z < sizeZ; z++) {
            double finalHeight = 0.0;

            for (int i = 0; i < 4; i++) {
                // Current corner vector
                double cornerX, cornerZ;

                switch (i) {
                case 0: cornerX = x + 0.5; cornerZ = z + 0.5; break;
                case 1: cornerX = sizeX - x - 0.5; cornerZ = z + 0.5; break;
                case 2: cornerX = x + 0.5; cornerZ = sizeZ - z - 0.5; break;
                case 3: cornerX = sizeX - x - 0.5; cornerZ = sizeZ - z - 0.5; break;
                }

                // Normalize the direction vector
                double magnitude = sqrt(cornerX * cornerX + cornerZ * cornerZ);
                double normX = cornerX / magnitude;
                double normZ = cornerZ / magnitude;

                // Get random vector from perlinNoiseRandomVector
                double randomAngle = perlinNoiseRandomVector[i] / 16.0 * 2.0 * Pie;
                double randX = cos(randomAngle);
                double randZ = sin(randomAngle);

                // Compute dot product
                double dotProduct = normX * randX + normZ * randZ;

                // Normalize to [0, 31]
                double normalizedHeight = ((dotProduct + 1.0) / 2.0) * 31.0;

                finalHeight += normalizedHeight;
            }

            // Average over 4 corners
            height[x][z] = static_cast<int>(finalHeight / 4.0);

            for (int y = 0; y < height[x][z]; y++)
            {
                m_World.VoxelIDs[glm::vec3(x,y,z)] =y%8+1;
            }
            
        }
    }
}

void SimulationLayer::OnUpdate(Timestep dt)
{

}
