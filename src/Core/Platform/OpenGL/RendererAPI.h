#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/VertexArray.h"

namespace Botanica::Renderer::OpenGL
{
    class RendererAPI : public Renderer::RendererAPI
    {
    public:
        RendererAPI();
        ~RendererAPI();

        void DrawIndexed(size_t count, size_t offset) const override;
        void SetClearColor(glm::vec4 color) const override;
        void ClearScreen() const override;
    };
}