#pragma once

#include "Shader.h"
#include "VertexArray.h"

namespace Botanica
{
    class Renderer
    {
    public:
        void BeginScene();
        void Submit(const Shader &shader, const VertexArray &mesh);
        void EndScene();
    };
}