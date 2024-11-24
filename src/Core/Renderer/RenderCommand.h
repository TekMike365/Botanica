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

        static void DrawIndexed(size_t count, size_t offset);
        static void ClearScreen();

        static void Execute();

    private:
        static RendererAPI *s_Renderer;
    };
}