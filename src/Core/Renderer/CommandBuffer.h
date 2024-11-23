#pragma once

#include <glm/glm.hpp>

#include "Shader.h"
#include "VertexArray.h"
#include "RendererAPI.h"

namespace Botanica::Renderer
{
    class CommandBuffer
    {
    public:
        static void SetShader(std::shared_ptr<Shader> shader);
        static void SetVertexArray(std::shared_ptr<VertexArray> va);
        static void SetClearColor(glm::vec4 color);

        static void DrawIndexed(size_t count, size_t offset);
        static void ClearScreen();

    private:
        static RendererAPI *s_Renderer;
    };
}