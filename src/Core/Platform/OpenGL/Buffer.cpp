#include "btpch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace Botanica
{
    IndexBuffer *IndexBuffer::Create(uint32_t size, const uint32_t *data)
    {
        return new OpenGL::IndexBuffer(size, data);
    }

    VertexBuffer *Botanica::VertexBuffer::Create(uint32_t size, const void *data)
    {
        return new OpenGL::VertexBuffer(size, data);
    }
}

namespace Botanica::OpenGL
{
    IndexBuffer::IndexBuffer(uint32_t size, const uint32_t *data)
    {
        glCreateBuffers(1, &m_ID);
        //TODO: abstract buffer usage
        glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
    }

    IndexBuffer::~IndexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void IndexBuffer::Bind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_ID);
    }

    void IndexBuffer::Unbind() const
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    VertexBuffer::VertexBuffer(uint32_t size, const void *data)
        :m_Layout({})
    {
        glCreateBuffers(1, &m_ID);
        //TODO: abstract buffer usage
        glNamedBufferData(m_ID, size, data, GL_STATIC_DRAW);
    }

    VertexBuffer::~VertexBuffer()
    {
        glDeleteBuffers(1, &m_ID);
    }

    void VertexBuffer::Bind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, m_ID);
    }

    void VertexBuffer::Unbind() const
    {
        glBindBuffer(GL_ARRAY_BUFFER, 0);
    }
}