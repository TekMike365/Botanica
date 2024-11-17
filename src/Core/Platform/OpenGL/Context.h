#pragma once

#include "Core/Renderer/GraphicsContext.h"

struct GLFWwindow;

namespace Botanica::Renderer::OpenGL
{
    class Context : public GraphicsContext
    {
    public:
        Context(GLFWwindow *windowHandle);

        void Init() override;
        void SwapBuffers() override;
    private:
        GLFWwindow *m_WindowHandle;
    };
}