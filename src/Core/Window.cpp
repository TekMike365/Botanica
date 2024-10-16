#include "btpch.h"

#include <glad/glad.h>

#include "Window.h"
#include "Core.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

namespace Botanica
{
    static bool s_GLFWInitialized;
    static bool s_GlInvalidOperationCalled = false;

    static void GLFWErrorCallback(int error, const char* description)
    {
        BT_ERROR("GLFW error ({}): {}", error, description);
    }

    static void APIENTRY GLErrorCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar* message, const void* userParam)
    {
        if (id == GL_INVALID_OPERATION && !s_GlInvalidOperationCalled)
            s_GlInvalidOperationCalled = true;
        else if (id == GL_INVALID_OPERATION)
            return;

        std::string msg(message, length);
        switch (severity)
        {
        case GL_DEBUG_SEVERITY_NOTIFICATION:
            BT_TRACE("OpenGL ({}): {}", id, msg);
            break;
        case GL_DEBUG_SEVERITY_LOW:
            BT_INFO("OpenGL ({}): {}", id, msg);
            break;
        case GL_DEBUG_SEVERITY_MEDIUM:
            BT_WARN("OpenGL ({}): {}", id, msg);
            break;
        case GL_DEBUG_SEVERITY_HIGH:
            BT_ERROR("OpenGL ({}): {}", id, msg);
            break;
        }
    }

    Window::Window(const WindowParams &params)
    {
        m_Data.Title = params.Title;
        m_Data.Width = params.Width;
        m_Data.Height = params.Height;
        BT_INFO("Creating window: {} ({}, {})", m_Data.Title, m_Data.Width, m_Data.Height);

        if (!s_GLFWInitialized)
        {
            int success = glfwInit();
            BT_ASSERT(success, "GLFW failed to initialize.");
            glfwSetErrorCallback(GLFWErrorCallback);
            s_GLFWInitialized = true;
        }

        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);

        m_Window = glfwCreateWindow(m_Data.Width, m_Data.Height, m_Data.Title.c_str(), 0, 0);
        BT_ASSERT(m_Window, "GLFW failed to create window.");

        glfwMakeContextCurrent(m_Window);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BT_ASSERT(status, "Glad it fucked up!");

        glDebugMessageCallback(GLErrorCallback, nullptr);

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

            glViewport(0, 0, width, height);

            WindowResizeEvent event(width, height);
            data.EventCallback(event);
        });

        glfwSetWindowFocusCallback(m_Window, [](GLFWwindow* window, int focused)
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
            }
        });

        glfwSetKeyCallback(m_Window, [](GLFWwindow* window, int key, int scancode, int action, int mods)
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
            }
        });

        glfwSetCursorPosCallback(m_Window, [](GLFWwindow* window, double xpos, double ypos)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseMovedEvent event((float)xpos, (float)ypos, (float)xpos / data.Width, (float)ypos / data.Height);
            data.EventCallback(event);
        });

        glfwSetScrollCallback(m_Window, [](GLFWwindow* window, double xoffset, double yoffset)
        {
            WindowData& data = *(WindowData*)glfwGetWindowUserPointer(window);

            MouseScrolledEvent event((float)xoffset, (float)yoffset);
            data.EventCallback(event);
        });

        glfwSetMouseButtonCallback(m_Window, [](GLFWwindow* window, int button, int action, int mods)
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
            }
        });
    }

    Window::~Window()
    {
        glfwDestroyWindow(m_Window);
    }

    void Window::OnUpdate()
    {
        glfwPollEvents();
        glfwSwapBuffers(m_Window);
    }

    void Window::SetVSync(bool enabled)
    {
        if (enabled)
            glfwSwapInterval(1);
        else
            glfwSwapInterval(0);

        m_Data.VSync = enabled;
    }
}
