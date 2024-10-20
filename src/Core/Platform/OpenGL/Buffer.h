#pragma once
#include "btpch.h"

#include "Core.h"

namespace Botanica::OpenGL
{
    enum class BufferType
    {
        None = 0,
        Index,
        Vertex
    };

    class Buffer
    {
    public:
        Buffer(BufferType type, uint32_t len, void *data);
        ~Buffer();

        void Bind() const;
        void Unbind() const;

        inline BufferType GetType() const { return m_Type; }

    private:
        uint32_t GetGLType(BufferType type) const;

    private:
        BufferType m_Type;
        u_int32_t m_GLBufferType;
        uint32_t m_ID;
    };
}