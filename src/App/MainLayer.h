#pragma once
#include <memory>

#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Camera.h"

#include "CameraController.h"
#include "World.h"

namespace App
{
    class MainLayer : public Botanica::Layer
    {
    public:
        MainLayer();
        ~MainLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate(Timestep dt) override;

    private:
        void RenderPass();
        void Setup();

    private:
        std::shared_ptr<Botanica::Renderer::VertexArray> m_VertexArray;
        std::shared_ptr<Botanica::Renderer::Shader> m_Shader;
        std::shared_ptr<Botanica::Renderer::Shader> m_ComputeShader;

        std::shared_ptr<Botanica::Renderer::Buffer> m_VoxelBuffer;

        Botanica::LayerStack m_ObjStack;
        CameraController *m_CameraController;
        World *m_World;
    };
}
