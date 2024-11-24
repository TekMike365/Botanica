#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "Shader.h"
#include "VertexArray.h"

namespace Botanica::Renderer
{
    class RendererAPI
    {
    public:
        virtual void SetClearColor(glm::vec4 color) const = 0;
        virtual void SetShader(std::shared_ptr<Shader> shader) const = 0;
        virtual void SetVertexArray(std::shared_ptr<VertexArray> va) const = 0;

        virtual void DrawIndexed(size_t count, size_t offset) const = 0;
        virtual void ClearScreen() const = 0;

        static RendererAPI *Create();
    };
}