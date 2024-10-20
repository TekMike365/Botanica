#pragma once

#include <glm/glm.hpp>

namespace Botanica
{
    class RendererAPI
    {
    public:
        virtual void DrawIndexed() const = 0;
        virtual void SetClearColor(glm::vec4 color) const = 0;
        virtual void ClearScreen() const = 0;

        static RendererAPI *Create();
    };
}