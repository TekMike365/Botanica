#pragma once

#include "Shader.h"
#include "VertexArray.h"

namespace Botanica
{
    class Renderer
    {
    public:
        static void BeginScene();
        static void Submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray);
        static void EndScene();
    };
}