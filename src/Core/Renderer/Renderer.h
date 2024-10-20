#pragma once

#include "Shader.h"
#include "VertexArray.h"

namespace Botanica
{
    class Renderer
    {
    public:
        static void BeginScene();
        static void Submit(const Shader &shader, const VertexArray &mesh);
        static void EndScene();
    };
}