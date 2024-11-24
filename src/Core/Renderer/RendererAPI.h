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
    };

    class RendererAPI
    {
    public:
        virtual void SetClearColor(glm::vec4 color) const = 0;
        virtual void SetRenderState(const RenderState& state) = 0;
        virtual void SetShaderUniforms(const std::vector<Uniform> &uniforms) const = 0;

        virtual void DrawIndexed(size_t count, size_t offset) const = 0;
        virtual void ClearScreen() const = 0;

        static RendererAPI *Create();
    };
}