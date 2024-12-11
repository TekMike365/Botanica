#pragma once

#include <memory>

#include "Layer.h"
#include "Components/Camera.h"
#include "Components/Vector2D.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

class RenderingLayer : public Layer
{
public:
    RenderingLayer(Camera &cam, Vector2D<uint32_t> &world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    std::shared_ptr<Renderer::VertexArray> m_VertexArray;
    std::shared_ptr<Renderer::Shader> m_Shader;

    Camera &m_Camera;
    Vector2D<uint32_t> &m_World;
};