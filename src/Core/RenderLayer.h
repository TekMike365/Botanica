#pragma once

#include <memory>

#include "Layer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"
#include "Renderer/Camera.h"

namespace Botanica
{
    class RenderLayer : public Layer
    {
    public:
        RenderLayer();
        ~RenderLayer();

        virtual void OnUpdate(Timestep dt) override;
    
    private:
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<Camera> m_Camera;
    };
}
