#pragma once
#include <memory>

#include "Core/Layer.h"
#include "Core/LayerStack.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Camera.h"

#include "CameraController.h"

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
        void Setup();

        void TestRenderPass();

    private:
        std::shared_ptr<Botanica::Renderer::VertexArray> m_VertexArray;
        std::shared_ptr<Botanica::Renderer::Shader> m_Shader;

        Botanica::LayerStack m_ObjStack;
        CameraController *m_CameraController;
    };
}
