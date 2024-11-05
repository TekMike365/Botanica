#include "btpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Botanica
{
    VertexArray *VertexArray::Create(VertexBuffer *vb, IndexBuffer *ib)
    {
        return new OpenGL::VertexArray(vb, ib);
    }
}

namespace Botanica::OpenGL
{
    GLenum ShaderDataTypeToOpenGLBaseType(ShaderDataType type)
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

        BT_CORE_ASSERT(false, "Unknown SaderDataType.");
        return GL_FALSE;
    }

    VertexArray::VertexArray(VertexBuffer *vb, IndexBuffer *ib)
        : m_VertexBuffer(vb), m_IndexBuffer(ib)
    {
        glGenVertexArrays(1, &m_ID);
        Bind();
        m_VertexBuffer->Bind();
        m_IndexBuffer->Bind();

        uint32_t index = 0;
        const BufferLayout &layout = m_VertexBuffer->GetLayout();
        for (const BufferElement &element : layout)
        {
            glVertexAttribPointer(index, element.GetElementCount(), ShaderDataTypeToOpenGLBaseType(element.Type), element.Normalized, layout.GetStride(), (void *)element.Offset);
            glEnableVertexAttribArray(index);
            index++;
        }

        Unbind();
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
}
