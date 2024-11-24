#pragma once
#include <memory>

#include "Core/Layer.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Camera.h"

namespace App
{
    class MainLayer : public Botanica::Layer
    {
    public:
        MainLayer();
        ~MainLayer();

        virtual void OnAttach() override;
        virtual void OnUpdate(Botanica::Timestep dt) override;

    private:
        void HandleInput(Botanica::Timestep dt);
        void Render();

        void Setup();

    private:
        std::shared_ptr<Botanica::Renderer::VertexArray> m_VertexArray;
        std::shared_ptr<Botanica::Renderer::Shader> m_Shader;
        std::shared_ptr<Botanica::Renderer::Camera> m_Camera;

        glm::vec2 m_LastMousePos;
    };
}
