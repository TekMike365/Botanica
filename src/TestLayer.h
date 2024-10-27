#pragma once

#include <memory>

#include "Core/Layer.h"
#include "Core/Renderer/VertexArray.h"
#include "Core/Renderer/Shader.h"
#include "Core/Renderer/Camera.h"

namespace Botanica
{
    class TestLayer : public Layer
    {
    public:
        TestLayer();
        ~TestLayer();

        virtual void OnUpdate(Timestep dt) override;

    private:
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Camera> m_Camera;

        glm::vec2 m_LastMousePos;
    };
}
