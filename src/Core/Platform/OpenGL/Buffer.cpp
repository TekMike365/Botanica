#include "btpch.h"
#include "Buffer.h"

#include <glad/glad.h>

#include "Core.h"

namespace Botanica::OpenGL
{
    Buffer::Buffer(BufferType type, uint32_t len, void *data)
        : m_Type(type), m_GLBufferType(GetGLType(type))
    {
        glCreateBuffers(1, &m_ID);
        // TODO: Abstract buffer usage
        glNamedBufferData(m_ID, len, data, GL_STATIC_DRAW);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void Buffer::Bind() const
    {
        glBindBuffer(m_GLBufferType, m_ID);
    }

    void Buffer::Unbind() const
    {
        glBindBuffer(m_GLBufferType, 0);
    }

    uint32_t Buffer::GetGLType(BufferType type) const
    {
        switch (type)
        {
        case BufferType::Index:
            return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::Vertex:
            return GL_ARRAY_BUFFER;
        }
        return 0;
    }

    VertexArray::VertexArray()
        : m_BindIndex(0)
    {
        glCreateVertexArrays(1, &m_ID);
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
    }

    void VertexArray::Unbind() const
    {
        glBindVertexArray(0);
    }
    
    void VertexArray::BindBuffer(const Buffer &buffer)
    {
        Bind();
        buffer.Bind();
    }
}
