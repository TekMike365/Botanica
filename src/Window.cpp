#include "pch.h"
#include "Window.h"

#include "Event/Event.h"
#include "Event/WindowEvent.h"
#include "Event/KeyboardEvent.h"
#include "Event/MouseEvent.h"

#include "Renderer/Renderer.h"

static bool s_GLFWInitialized = false;

static void GLFWErrorCallback(int error, const char *description)
{
    BT_DLOG_ERROR("GLFW error ({}): {}", error, description);
}

Window::Window(const WindowProps &props)
{
    m_WindowData.Title = props.Title;
    m_WindowData.Height = props.Height;
    m_WindowData.Width = props.Width;

    BT_DLOG_INFO("Creating window {} ({}, {})", props.Title, props.Width, props.Height);

    if (!s_GLFWInitialized)
    {
        int success = glfwInit();
        BT_ASSERT(success, "GLFW failed to initialize.");
        glfwSetErrorCallback(GLFWErrorCallback);
        s_GLFWInitialized = true;
    }

    m_Window = glfwCreateWindow(m_WindowData.Width, m_WindowData.Height, m_WindowData.Title.c_str(), 0, 0);
    BT_ASSERT(m_Window, "GLFW failed to create window.");

    Init();

    BT_DLOG_INFO("Window Created!");
}

Window::~Window()
{
    glfwDestroyWindow(m_Window);
}

void Window::Init()
{
    Renderer::Init(m_Window);

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

void Window::OnUpdate()
{
    glfwPollEvents();
    Renderer::SwapBuffers();
}

void Window::SetVSync(bool enabled)
{
    if (enabled)
        glfwSwapInterval(1);
    else
        glfwSwapInterval(0);

    m_WindowData.VSync = enabled;
}
