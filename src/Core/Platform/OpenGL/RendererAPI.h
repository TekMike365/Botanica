#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/VertexArray.h"

namespace Botanica::OpenGL
{
    class RendererAPI : public Botanica::RendererAPI
    {
    public:
        RendererAPI();
        ~RendererAPI();

        void DrawIndexed(std::shared_ptr<VertexArray> vertexArray) const override;
        void SetClearColor(glm::vec4 color) const override;
        void ClearScreen() const override;
    };
}