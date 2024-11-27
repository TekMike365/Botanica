#include "btpch.h"
#include "VertexArray.h"

#include <glad/glad.h>

namespace Botanica::Renderer
{
    std::shared_ptr<VertexArray> VertexArray::Create(BufferSPtr vb, BufferSPtr ib)
    {
        return std::make_shared<OpenGL::VertexArray>(vb, ib);
    }
}

namespace Botanica::Renderer::OpenGL
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

        BT_CORE_ASSERT(false, "Unknown SaderDataType.");
        return GL_FALSE;
    }

    VertexArray::VertexArray(BufferSPtr vb, BufferSPtr ib)
        : m_VertexBuffer(vb), m_IndexBuffer(ib)
    {
        glGenVertexArrays(1, &m_ID);
        Bind();
        m_VertexBuffer->Bind(BufferType::Vertex);
        m_IndexBuffer->Bind(BufferType::Index);

        uint32_t stride = vb->GetLayout().GetStride();
        uint32_t index = 0;
        for (const BufferLayout::DataType &e : vb->GetLayout())
        {
            glVertexAttribPointer(index, GetSubTypeCount(e.Type), GetGLType(e.Type), e.Normalized, stride, (const void *)e.Offset);
            glEnableVertexAttribArray(index);
            index++;
        }

        m_VertexBuffer->Unbind(BufferType::Vertex);
        m_IndexBuffer->Unbind(BufferType::Index);
        Unbind();
    }

    VertexArray::~VertexArray()
    {
        glDeleteVertexArrays(1, &m_ID);
    }

    void VertexArray::Bind() const
    {
        glBindVertexArray(m_ID);
        m_IndexBuffer->Bind(BufferType::Index);
    }

    void VertexArray::Unbind() const
    {
        m_IndexBuffer->Unbind(BufferType::Index);
        glBindVertexArray(0);
    }
}
