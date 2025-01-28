#pragma once

#include <memory>

#include "Layer.h"
#include "Components/Camera.h"
#include "Components/World.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Buffer.h"
#include "Renderer/Shader.h"

#include "Event/KeyboardEvent.h"
#include "Event/WindowEvent.h"

class RenderingLayer : public Layer
{
public:
    RenderingLayer(Camera &cam, std::shared_ptr<World> world);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;
    virtual void OnEvent(Event &e) override;

private:
    bool OnKeyReleased(KeyReleasedEvent &e);
    bool OnWindowResized(WindowResizeEvent &e);

private:
    bool m_DrawWireframe = false;
    float m_VoxelScale = 1.0f / 8.0f;

    std::shared_ptr<Renderer::VertexArray> m_WorldBoundsVA;
    std::shared_ptr<Renderer::Shader> m_WorldBoundsShader;

    std::shared_ptr<Renderer::Buffer> m_VoxelBuffer;
    std::shared_ptr<Renderer::VertexArray> m_VoxelVA;
    std::shared_ptr<Renderer::Shader> m_VoxelGenCShader;
    std::shared_ptr<Renderer::Shader> m_VoxelRenderShader;

    Camera &m_Camera;
    std::shared_ptr<World> m_World;
};