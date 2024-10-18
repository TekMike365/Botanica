#include "CrossPlatformWindow.h"

#include "Core.h"
#include "Log.h"

namespace Botanica
{
    static bool s_GLFWInitialized = false;

    Window *Window::Create(const WindowProps &props)
    {
        return new LinuxWindow(props);
    }

    LinuxWindow::LinuxWindow(const WindowProps &props)
    {
        m_WindowData.Title = props.Title;
        m_WindowData.Height = props.Height;
        m_WindowData.Width = props.Width;

        BT_CORE_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            BT_CORE_ASSERT(success, "GLFW failed to initialize.");
            s_GLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), 0, 0);
        BT_CORE_ASSERT(m_Window, "GLFW failed to create window.");

        glfwMakeContextCurrent(m_Window);

        SetVSync(true);
    }

    LinuxWindow::~LinuxWindow()
    {
        glfwDestroyWindow(m_Window);
    }

    
    void LinuxWindow::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void LinuxWindow::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_WindowData.VSync = enabled;
    }
}