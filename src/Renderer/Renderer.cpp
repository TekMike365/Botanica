#include "pch.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Renderer
{
    static GLFWwindow *s_WindowHandle = nullptr;
}

static void APIENTRY GLErrorCallback(GLenum source, GLenum type, GLenum id, GLenum severity, GLsizei length, const GLchar *message, const void *userParam)
{
    std::string msg(message, length);
    switch (severity)
    {
    case GL_DEBUG_SEVERITY_NOTIFICATION:
        BT_DLOG_TRACE("OpenGL ({}): {}", id, msg);
        break;
    case GL_DEBUG_SEVERITY_LOW:
        BT_DLOG_INFO("OpenGL ({}): {}", id, msg);
        break;
    case GL_DEBUG_SEVERITY_MEDIUM:
        BT_DLOG_WARN("OpenGL ({}): {}", id, msg);
        break;
    case GL_DEBUG_SEVERITY_HIGH:
        BT_DLOG_ERROR("OpenGL ({}): {}", id, msg);
        break;
    }
}

void Renderer::Init(GLFWwindow *windowHandle)
{
    s_WindowHandle = windowHandle;

    glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
    glfwMakeContextCurrent(s_WindowHandle);

    int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
    BT_ASSERT(status, "Glad it fucked up!");

    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback(GLErrorCallback, nullptr);

    BT_DLOG_INFO("OpenGL information:");
    BT_DLOG_INFO("    version: {}", (const char *)glGetString(GL_VERSION));
    int size = 0;
    glGetIntegerv(GL_MAX_COMPUTE_WORK_GROUP_INVOCATIONS, &size);
    BT_DLOG_WARN("    max CS group size: {}", size);
    glEnable(GL_DEPTH_TEST);
}

void Renderer::SwapBuffers()
{
    glfwSwapBuffers(s_WindowHandle);
}