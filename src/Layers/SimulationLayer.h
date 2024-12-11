#pragma once

#include <memory>

#include "Layer.h"
#include "Components/Camera.h"
#include "Components/World.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

class SimulationLayer : public Layer
{
public:
    SimulationLayer(World<uint32_t> &world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    World<uint32_t> &m_World;
};