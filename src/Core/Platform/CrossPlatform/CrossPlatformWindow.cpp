#include "CrossPlatformWindow.h"

#include <glad/glad.h>

#include "Core.h"
#include "Log.h"

#include "Core/Event/Event.h"
#include "Core/Event/WindowEvent.h"
#include "Core/Event/KeyboardEvent.h"
#include "Core/Event/MouseEvent.h"

namespace Botanica
{
    static bool s_GLFWInitialized = false;

    static void GLFWErrorCallback(int error, const char *description)
    {
        BT_CORE_ERROR("GLFW error ({}): {}", error, description);
    }

    static void APIENTRY GLErrorCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        if (id == GL_INVALID_OPERATION)
            return;

        std::string msg(message, length);
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            BT_CORE_TRACE("OpenGL ({}): {}", id, msg);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            BT_CORE_INFO("OpenGL ({}): {}", id, msg);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            BT_CORE_WARN("OpenGL ({}): {}", id, msg);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            BT_CORE_ERROR("OpenGL ({}): {}", id, msg);
            break;
        }
    }

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
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), 0, 0);
        BT_CORE_ASSERT(m_Window, "GLFW failed to create window.");

        glfwMakeContextCurrent(m_Window);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BT_CORE_ASSERT(status, "Glad it fucked up!");
        glDebugMessageCallback(GLErrorCallback, nullptr);

        BT_CORE_INFO("OpenGL version: {}", (const char*)glGetString(GL_VERSION));
        glEnable(GL_DEPTH_TEST);

        glfwSetWindowUserPointer(m_Window, &m_WindowData);
        SetVSync(true);

        glfwSetWindowCloseCallback(m_Window, [](GLFWwindow *window)
                                   {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            WindowCloseEvent event;
            data.EventCallback(event); });

        glfwSetWindowSizeCallback(m_Window, [](GLFWwindow *window, int width, int height)
                                  {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);
            data.Width = width;
            data.Height = height;

            WindowResizeEvent event(width, height);
            data.EventCallback(event); });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow *window, int focused)
                                   {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            if (focused)
            {
                WindowFocusEvent event;
                data.EventCallback(event);
            }
            else
            {
                WindowLostFocusEvent event;
                data.EventCallback(event);
            } });

        glfwSetKeyCallback(m_Window, [](GLFWwindow *window, int key, int scancode, int action, int mods)
                           {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    KeyPressedEvent event(key, 0);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_REPEAT:
                {
                    KeyPressedEvent event(key, 1);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    KeyReleasedEvent event(key);
                    data.EventCallback(event);
                    break;
                }
            } });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow *window, double xpos, double ypos)
                                 {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xpos, (float)ypos, (float)xpos / data.Width, (float)ypos / data.Height);
            data.EventCallback(event); });

        glfwSetScrollCallback(m_Window, [](GLFWwindow *window, double xoffset, double yoffset)
                              {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xoffset, (float)yoffset);
            data.EventCallback(event); });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow *window, int button, int action, int mods)
                                   {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            switch (action)
            {
                case GLFW_PRESS:
                {
                    MouseButtonPressedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
                case GLFW_RELEASE:
                {
                    MouseButtonReleasedEvent event(button);
                    data.EventCallback(event);
                    break;
                }
            } });
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