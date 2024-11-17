#pragma once

#include <glm/glm.hpp>

#include "RendererAPI.h"

namespace Botanica::Renderer
{
    class RenderCommand
    {
    public:
        static void DrawIndexed(std::shared_ptr<VertexArray> vertexArray);
        static void SetClearColor(glm::vec4 color);
        static void ClearScreen();

    private:
        static RendererAPI *s_Renderer;
    };
}