#include "btpch.h"
#include "Context.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Botanica::OpenGL
{
    static void APIENTRY GLErrorCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
    {
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

    Context::Context(GLFWwindow *windowHandle)
        : m_WindowHandle(windowHandle)
    {
    }

    void Context::Init()
    {
        glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
        glfwMakeContextCurrent(m_WindowHandle);

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BT_CORE_ASSERT(status, "Glad it fucked up!");

        glEnable(GL_DEBUG_OUTPUT);
        glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
        glDebugMessageCallback(GLErrorCallback, nullptr);

        BT_CORE_INFO("OpenGL version: {}", (const char *)glGetString(GL_VERSION));
        glEnable(GL_DEPTH_TEST);
    }

    void Context::SwapBuffers()
    {
        glfwSwapBuffers(m_WindowHandle);
    }
}