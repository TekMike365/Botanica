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


    struct RenderAPIInfo
    {
        int GLVersionMajor = 0;
        int GLVersionMinor = 0;
        int MaxComputeWorkGroupInvocations = 0;
        glm::ivec3 MaxComputeWorkGroupCount = glm::ivec3(0.0f);
        glm::ivec3 MaxComputeWorkGroupSize = glm::ivec3(0.0f);
    };

    const RenderAPIInfo &GetAPIInfo();

    uint8_t GetSubTypeCount(ShaderDataType type);
    uint32_t GetTypeSize(ShaderDataType type);

    void Init(GLFWwindow *windowHandle);
    void SwapBuffers();

    void SetClearColor(glm::vec4 col);
    void SetMemoryBarrier(MemoryBarrier barrier);

    void DispatchCompute(uint32_t groups_x = 1, uint32_t groups_y = 1, uint32_t groups_z = 1);
    void DrawIndexed(size_t count, size_t offset = 0);
    void ClearScreen();
}