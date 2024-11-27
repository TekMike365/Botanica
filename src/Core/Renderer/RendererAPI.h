#pragma once

#include <memory>
#include <array>
#include <glm/glm.hpp>

#include "Shader.h"
#include "VertexArray.h"

namespace Botanica::Renderer
{
    struct RenderState
    {
        std::shared_ptr<Shader> ShaderPtr;
        std::shared_ptr<VertexArray> VertexArrayPtr;
        glm::vec4 ClearColor = glm::vec4(0.0f, 0.0f, 0.0f, 1.0f);
    };

    class RendererAPI
    {
    public:
        virtual void SetRenderState(const RenderState& state) = 0;
        virtual void SetShaderUniforms(const std::vector<Uniform> &uniforms) = 0;
        virtual void SetShaderUniformBuffers(const std::vector<UniformBuffer> &uniformBuffers) = 0;

        virtual void DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z) = 0;
        virtual void DrawIndexed(size_t count, size_t offset) = 0;
        virtual void ClearScreen() = 0;

        virtual void Execute() = 0;

        virtual bool IsRendering() const = 0;

        static RendererAPI *Create();
    };
}