#include "btpch.h"
#include "GLFWWindow.h"
#include "Core.h"

namespace Botanica
{
    static bool s_isGLFWInitialized;

    Window* Window::CreateWindow(const WindowParams& params)
    {
        return new GLFWWindow(params);
    }

    GLFWWindow::GLFWWindow(const WindowParams &params)
    {
        m_Data.Title = params.Title;
        m_Data.Width = params.Width;
        m_Data.Height = params.Height;

        if (!s_isGLFWInitialized)
        {
            int status = glfwInit();
            BT_ASSERT(status, "GLFW failed to initialize.");

            s_isGLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), 0, 0);
        BT_ASSERT(m_Window, "GLFW failed to create window.");

        glfwMakeContextCurrent(m_Window);
        SetVSync(true);
    }

    GLFWWindow::~GLFWWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    void GLFWWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void GLFWWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }
}
