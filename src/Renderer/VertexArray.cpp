#include "VertexArray.h"

namespace Botanica::Renderer
{

    VertexArray::VertexArray()
        :m_ID(0)
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

    void VertexArray::AddVertexBuffer(const VertexBuffer &vb, const VertexLayout &layout)
    {
        vb.Bind();
        const std::vector<VertexElement>& elements = layout.GetElements();
        uint32_t offset = 0;
        for (GLuint i = 0; i <  elements.size(); i++)
        {
            const VertexElement& element = elements[i];
            glVertexAttribPointer(i, element.Count, element.Type, element.Normalized, layout.GetStride(), (void*)offset);
            glEnableVertexAttribArray(i);
            offset += element.Size;
        }
    }

}
