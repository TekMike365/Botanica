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
        virtual void SetRenderState(const RenderState& state) override;
        virtual void SetShaderUniforms(const std::vector<Uniform> &uniforms) const override;

        virtual void DrawIndexed(size_t count, size_t offset) const override;
        virtual void ClearScreen() const override;
    
    private:
        RenderState m_RenderState;
    };
}