#include "VertexArray.h"

namespace Botanica
{

    VertexArray::VertexArray()
        :m_ID(0), m_Stride(0)
    {
        glGenVertexArrays(1, &m_ID);
        Bind();
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

    void VertexArray::AddVertexBuffer(const VertexBuffer &vb, const VertexBufferLayout &layout)
    {
        vb.Bind();
        const std::vector<VertexBufferElement>& vbes = layout.GetVBEs();
        for (GLuint i = 0; i <  vbes.size(); i++)
        {
            const VertexBufferElement& vbe = vbes[i];
            m_Stride += vbe.Size;
            glVertexAttribPointer(i, vbe.Count, vbe.Type, vbe.Normalized, m_Stride, (void *)0);
            glEnableVertexAttribArray(i);
        }
    }

}
