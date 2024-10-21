#pragma once

#include <memory>
#include <glm/glm.hpp>

#include "VertexArray.h"

namespace Botanica
{
    class RendererAPI
    {
    public:
        virtual void DrawIndexed(std::shared_ptr<VertexArray> vertexArray) const = 0;
        virtual void SetClearColor(glm::vec4 color) const = 0;
        virtual void ClearScreen() const = 0;

        static RendererAPI *Create();
    };
}