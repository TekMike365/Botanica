#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Botanica::Renderer
{
    class RendererAPI
    {
    public:
        virtual void DrawIndexed(size_t count, size_t offset) const = 0;
        virtual void SetClearColor(glm::vec4 color) const = 0;
        virtual void ClearScreen() const = 0;

        static RendererAPI *Create();
    };
}