#pragma once

#include <glm/glm.hpp>

#include "RendererAPI.h"

namespace Botanica::Renderer
{
    class CommandBuffer
    {
    public:
        static void DrawIndexed(size_t count, size_t offset);
        static void SetClearColor(glm::vec4 color);
        static void ClearScreen();

    private:
        static RendererAPI *s_Renderer;
    };
}