#pragma once

#include <memory>
#include <vector>
#include <glm/glm.hpp>

#include "RendererAPI.h"
#include "Shader.h"
#include "Buffer.h"
#include "VertexArray.h"

namespace Botanica::Renderer
{
    class CommandBuffer
    {
    public:
        static void SetClearColor(glm::vec4 color);
        static void SetVertexArray(std::shared_ptr<VertexArray> va);
        static void SetShader(std::shared_ptr<Shader> shader);

        static void DrawIndexed(size_t count, size_t offset);
        static void ClearScreen();

        static void Execute();

    private:
        static RendererAPI *s_Renderer;
    };
}