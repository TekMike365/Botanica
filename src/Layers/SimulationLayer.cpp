#include "SimulationLayer.h"
#include "Platform.h"

SimulationLayer::SimulationLayer(World &world)
    :m_World(world)
{
}

void SimulationLayer::OnAttach()
{

    std::vector<int> zValues;

    srand(time(NULL));
    std::vector<int> perlinNoiseRandomVector;
    for (int i = 0; i < 4; i++)
    {
        perlinNoiseRandomVector.emplace_back(rand()%16);
    }
    double Pie = 3.14159265358979323846;

    for (int x = 0; x < m_World.VoxelIDs.GetSize().x; x++)
    {
        for (int z = 0; z < m_World.VoxelIDs.GetSize().z; z++)
        {
            double protoHeight = 0;
            std::array<double,4>circleConstants = {1/sqrt(pow(z+1,2)+pow(x+1,2)),1/sqrt(pow(z+1,2)+pow(m_World.VoxelIDs.GetSize().x-x+1,2)),1/sqrt(pow(x+1,2)+pow(m_World.VoxelIDs.GetSize().z-z+1,2)),1/sqrt(pow(m_World.VoxelIDs.GetSize().x-x+1,2)+pow(m_World.VoxelIDs.GetSize().z-z+1,2))};

            for (int i = 0; i < 4; i++)
            {
                protoHeight += (sin(perlinNoiseRandomVector[i]/8*Pie)*z*circleConstants[i]+cos(perlinNoiseRandomVector[i]/8*Pie)*x*circleConstants[i])*15.5+15.5;
            }
            
            for (int y = 0; y < (int)protoHeight/4; y++)
            {
                m_World.VoxelIDs[glm::uvec3(x,y,z)] = y%8+1;
            }
        }
    }

}

void SimulationLayer::OnUpdate(Timestep dt)
{
}
