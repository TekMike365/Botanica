#pragma once

#include <vector>

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/VertexArray.h"

namespace Botanica::Renderer::OpenGL
{
    class RendererAPI : public Renderer::RendererAPI
    {
    public:
        RendererAPI() = default;
        ~RendererAPI() = default;

        virtual void SetRenderState(const RenderState& state) override;
        virtual void SetShaderUniforms(const std::vector<Uniform> &uniforms) override;
        virtual void SetShaderUploadBuffers(const std::vector<UploadBuffer> &uploadBuffers) override;

        virtual void DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z) override;
        virtual void DrawIndexed(size_t count, size_t offset) override;
        virtual void ClearScreen() override;

        virtual void Execute() override {}
        virtual inline bool IsRendering() const override { return false; }

    private:
        std::shared_ptr<Shader> m_Shader;
        std::shared_ptr<VertexArray> m_VertexArray;
    };
}