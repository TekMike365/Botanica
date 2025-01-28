#pragma once

#include "Layer.h"
#include "Components/World.h"
#include "Components/Plant.h"

class SimulationLayer : public Layer
{
public:
    SimulationLayer(std::shared_ptr<World> world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    void PlantAPlant(glm::uvec2 xzPos);
    void GenerateTerrain();

private:
    std::shared_ptr<World> m_World;
    std::vector<Plant> m_Plants;

    const float m_HeightScalar = 16.0;
    const int m_WaterLevel = 6;
};