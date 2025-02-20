#include "pch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Renderer
{
    GLenum GetGLType(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
        case ShaderDataType::Float2:
        case ShaderDataType::Float3:
        case ShaderDataType::Float4:
            return GL_FLOAT;
        case ShaderDataType::Int:
        case ShaderDataType::Int2:
        case ShaderDataType::Int3:
        case ShaderDataType::Int4:
            return GL_INT;
        case ShaderDataType::Bool:
            return GL_BOOL;
        }

        BT_ASSERT(false, "Unknown SaderDataType.");
        return GL_FALSE;
    }

    VertexArray::VertexArray(std::shared_ptr<Buffer> vb, std::shared_ptr<Buffer> ib)
        : m_VertexBuffer(vb), m_IndexBuffer(ib)
    {
        glGenVertexArrays(1, &m_ID);
        Bind();
        m_VertexBuffer->Bind();
        m_IndexBuffer->Bind();

        uint32_t stride = vb->GetLayout().GetStride();
        uint32_t index = 0;
        for (const BufferLayout::DataType &e : vb->GetLayout())
        {
            glVertexAttribPointer(index, GetSubTypeCount(e.Type), GetGLType(e.Type), e.Normalized, stride, (const void *)e.Offset);
            glEnableVertexAttribArray(index);
            index++;
        }

        m_VertexBuffer->Unbind();
        m_IndexBuffer->Unbind();
        Unbind();
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
        m_IndexBuffer->Bind();
    }

    void VertexArray::Unbind() const
    {
        m_IndexBuffer->Unbind();
        glBindVertexArray(0);
    }
}
