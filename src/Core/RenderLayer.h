#pragma once

#include <memory>

#include "Layer.h"
#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

namespace Botanica
{
    class RenderLayer : public Layer
    {
    public:
        RenderLayer();
        ~RenderLayer();

        virtual void OnUpdate();
    
    private:
        std::shared_ptr<VertexArray> m_VertexArray;
        std::shared_ptr<Shader> m_Shader;
    };
}
