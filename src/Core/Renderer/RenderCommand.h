#pragma once

#include <memory>
#include <cstdint>
#include <glm/glm.hpp>

#include "RendererAPI.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

namespace Botanica::Renderer
{
    class RenderCommand
    {
    public:
        static void SetRenderState(const RenderState& state);
        static void SetShaderUniforms(const std::vector<Uniform> &uniforms);
        static void SetShaderUniformBuffers(const std::vector<UniformBuffer> &uniformBuffers);

        static void DispatchCompute(uint32_t groups_x = 1, uint32_t groups_y = 1, uint32_t groups_z = 1);
        static void DrawIndexed(size_t count, size_t offset);
        static void ClearScreen();

        static void Execute();

        static bool IsRendering();

    private:
        static RendererAPI *s_Renderer;
    };
}