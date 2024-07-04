#include "IndexBuffer.h"

namespace Botanica::Renderer
{

    IndexBuffer::IndexBuffer(const uint32_t* data, uint32_t count)
        :m_ID(0), m_Count(count)
    {
        glGenBuffers(1, &m_ID);
        Bind();
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

}
