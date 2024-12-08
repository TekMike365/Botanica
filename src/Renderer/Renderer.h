#pragma once

#include <cstdint>
#include <GLFW/glfw3.h>

namespace Renderer
{
    enum class ShaderDataType
    {
        None = 0, Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4
    };

    uint8_t GetSubTypeCount(ShaderDataType type);
    uint32_t GetTypeSize(ShaderDataType type);

    void Init(GLFWwindow *windowHandle);
    void SwapBuffers();
}