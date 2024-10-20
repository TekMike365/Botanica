#pragma once

#include <glm/glm.hpp>

#include "RendererAPI.h"

namespace Botanica
{
    class RenderCommand
    {
    public:
        static void DrawIndexed();
        static void SetClearColor(glm::vec4 color);
        static void ClearScreen();

    private:
        static RendererAPI *s_Renderer;
    };
}