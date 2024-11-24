#pragma once

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/VertexArray.h"

namespace Botanica::Renderer::OpenGL
{
    class RendererAPI : public Renderer::RendererAPI
    {
    public:
        RendererAPI() = default;
        ~RendererAPI() = default;

        virtual void SetClearColor(glm::vec4 color) const override;
        virtual void SetShader(std::shared_ptr<Shader> shader) const override;
        virtual void SetVertexArray(std::shared_ptr<VertexArray> va) const override;

        virtual void DrawIndexed(size_t count, size_t offset) const override;
        virtual void ClearScreen() const override;
    };
}