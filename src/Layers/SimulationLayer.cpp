#include "Ffunctions.h"

SimulationLayer::SimulationLayer(World &world)
    :m_World(world)
{
}

void SimulationLayer::OnAttach()
{
    Generate();
    for (int X = 0; X < x; X++)
    {
        for (int Z = 0; Z < z; Z++)
        {
            for (int Y = 0; Y < y; Y++)
            {
                if (std::dynamic_pointer_cast<Soil>(enviroment[glm::uvec3(X,Y,Z)]))
                {
                    //asi  treba std::iota na enviroment
                    m_World.VoxelIDs[glm::uvec3(X,Y,Z)] = enviroment[glm::uvec3(Z,Y,Z)]->type;
                }
            }
        }
    }
    
}

void SimulationLayer::OnUpdate(Timestep dt)
{

}
