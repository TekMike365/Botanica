#include "btpch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace Botanica
{
    IndexBuffer *IndexBuffer::Create(uint32_t count, const uint32_t *data)
    {
        return new OpenGL::IndexBuffer(count, data);
    }

    VertexBuffer *Botanica::VertexBuffer::Create(uint32_t size, const void *data)
    {
        return new OpenGL::VertexBuffer(size, data);
    }
}

namespace Botanica::OpenGL
{
    IndexBuffer::IndexBuffer(uint32_t count, const uint32_t *data)
        : m_Count(count)
    {
        glGenBuffers(1, &m_ID);
        Bind();
        //TODO: abstract buffer usage
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, count * sizeof(uint32_t), data, GL_STATIC_DRAW);
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

    void IndexBuffer::UploadData(uint32_t offset, uint32_t size, const void *data) const
    {
        Bind();
        glBufferSubData(GL_ELEMENT_ARRAY_BUFFER, offset, size, data);
    }

    VertexBuffer::VertexBuffer(uint32_t size, const void *data)
    {
        glGenBuffers(1, &m_ID);
        Bind();
        //TODO: abstract buffer usage
        glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
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

    void VertexBuffer::UploadData(uint32_t offset, uint32_t size, const void *data) const
    {
        Bind();
        glBufferSubData(GL_ARRAY_BUFFER, offset, size, data);
    }
}
