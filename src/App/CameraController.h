#pragma once

#include "Core/Layer.h"
#include "Core/Renderer/Camera.h"

namespace App
{
    using namespace Botanica;

    class CameraController : public Layer
    {
    public:
        CameraController();

        virtual void OnAttach() override;
        virtual void OnUpdate(Timestep dt) override;

        inline Renderer::Camera &GetCamera() { return m_Camera; }

    private:
        Renderer::Camera m_Camera;
        glm::vec2 m_LastMousePos;
    };
}