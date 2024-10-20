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
        std::unique_ptr<VertexBuffer> m_VertexBuffer;
        std::unique_ptr<IndexBuffer> m_IndexBuffer;
        std::unique_ptr<VertexArray> m_VertexArray;
        std::unique_ptr<Shader> m_Shader;
    };
}
