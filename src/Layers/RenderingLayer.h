#pragma once

#include <memory>

#include "Layer.h"
#include "Components/Camera.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

class RenderingLayer : public Layer
{
public:
    RenderingLayer(Camera &cam);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    std::shared_ptr<Renderer::VertexArray> m_VertexArray;
    std::shared_ptr<Renderer::Shader> m_Shader;

    Camera &m_Camera;
};