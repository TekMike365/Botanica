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
    World &m_World;
};