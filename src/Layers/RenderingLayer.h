#pragma once

#include <memory>

#include "Layer.h"
#include "Components/Camera.h"
#include "Components/Vector3D.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

class RenderingLayer : public Layer
{
public:
    RenderingLayer(Camera &cam, Vector3D<uint32_t> &world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

private:
    std::shared_ptr<Renderer::Buffer> m_VoxelBuffer;
    std::shared_ptr<Renderer::VertexArray> m_VA;
    std::shared_ptr<Renderer::Shader> m_VoxelGenCShader;
    std::shared_ptr<Renderer::Shader> m_RenderShader;

    Camera &m_Camera;
    Vector3D<uint32_t> &m_World;
};