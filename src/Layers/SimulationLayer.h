#pragma once

#include "Layer.h"
#include "Components/World.h"

class SimulationLayer : public Layer
{
public:
    SimulationLayer(World &world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    void GenerateTerrain();

private:
    World &m_World;

    const float m_HeightScalar = 16.0;
    const int m_WaterLevel = 6;
};