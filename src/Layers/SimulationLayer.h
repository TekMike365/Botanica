#pragma once

#include <memory>

#include "Layer.h"
#include "Components/Camera.h"
#include "Components/Vector3D.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

class SimulationLayer : public Layer
{
public:
    SimulationLayer(Vector3D<uint32_t> &world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    Vector3D<uint32_t> &m_World;
};