#pragma once

#include "Core/Renderer/RendererAPI.h"

namespace Botanica::OpenGL
{
    class RendererAPI : public Botanica::RendererAPI
    {
    public:
        RendererAPI();
        ~RendererAPI();

        void DrawIndexed() const override;
        void SetClearColor(glm::vec4 color) const override;
        void ClearScreen() const override;
    };
}