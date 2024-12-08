#pragma once

#include <cstdint>
#include <GLFW/glfw3.h>
#include <glad/glad.h>

namespace Renderer
{
    enum class ShaderDataType
    {
        None = 0, Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4
    };

    enum MemoryBarrier
    {
        MemoryBarrierNone = 0,
        MemoryBarrierShaderStorage = GL_SHADER_STORAGE_BARRIER_BIT
    };

    uint8_t GetSubTypeCount(ShaderDataType type);
    uint32_t GetTypeSize(ShaderDataType type);

    void Init(GLFWwindow *windowHandle);
    void SwapBuffers();

    void SetClearColor(glm::vec4 col);
    void SetMemoryBarrier(MemoryBarrier barrier);

    void DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z);
    void DrawIndexed(size_t count, size_t offset);
    void ClearScreen();
}