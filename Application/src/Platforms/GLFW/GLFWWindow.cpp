#include "btpch.h"
#include "GLFWWindow.h"
#include "Core.h"

#include "Events/ApplicationEvent.h"

namespace Botanica
{
    static bool s_isGLFWInitialized;

    static void GLFWErrorCallback(int error, const char* description)
    {
        BT_ERROR("GLFW error ({}): {}", error, description);
    }

    Window* Window::CreateWindow(const WindowParams& params)
    {
        return new GLFWWindow(params);
    }

    GLFWWindow::GLFWWindow(const WindowParams &params)
    {
        m_Data.Title = params.Title;
        m_Data.Width = params.Width;
        m_Data.Height = params.Height;
        BT_INFO("Creating window: {} ({}, {})", m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_isGLFWInitialized)
        {
            int status = glfwInit();
            BT_ASSERT(status, "GLFW failed to initialize.");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_isGLFWInitialized = true;
        }

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), 0, 0);
        BT_ASSERT(m_Window, "GLFW failed to create window.");

        glfwMakeContextCurrent(m_Window);
        glfwSetWindowUserPointer(m_Window, &m_Data);
        SetVSync(true);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow* window)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event);
        });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow* window, int width, int height)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });
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
