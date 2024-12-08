#include "pch.h"
#include "Renderer.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

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

namespace Renderer
{
    static GLFWwindow *s_WindowHandle = nullptr;
}

uint8_t Renderer::GetSubTypeCount(ShaderDataType type)
{
    switch (type)
    {
    case ShaderDataType::Bool:
    case ShaderDataType::Int:
    case ShaderDataType::UInt:
    case ShaderDataType::Float:
        return 1;
    case ShaderDataType::Int2:
    case ShaderDataType::UInt2:
    case ShaderDataType::Float2:
        return 2;
    case ShaderDataType::Int3:
    case ShaderDataType::UInt3:
    case ShaderDataType::Float3:
        return 3;
    case ShaderDataType::Int4:
    case ShaderDataType::UInt4:
    case ShaderDataType::Float4:
        return 4;
    }

    BT_ASSERT(false, "Unknown SaderDataType.");
    return 0;
}

uint32_t Renderer::GetTypeSize(ShaderDataType type)
{
    if (type == ShaderDataType::Bool)
        return 1;
    
    return GetSubTypeCount(type) * 4;
}

void Renderer::Init(GLFWwindow *windowHandle)
{
    BT_ASSERT(!s_WindowHandle, "renderer initialised multiple times")
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

void Renderer::SetClearColor(glm::vec4 col)
{
    glClearColor(col.r, col.g, col.b, col.a);
}

void Renderer::SetMemoryBarrier(MemoryBarrier barrier)
{
    glMemoryBarrier(barrier);
}

void Renderer::DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z)
{
    glDispatchCompute(groups_x, groups_y, groups_z);
}

void Renderer::DrawIndexed(size_t count, size_t offset)
{
    glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void *)offset);
}

void Renderer::ClearScreen()
{
    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}
