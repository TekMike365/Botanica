#pragma once

#include "btpch.h"
#include <glad/glad.h>

#include "VertexBuffer.h"

namespace Botanica
{

    struct VertexBufferElement
    {
        GLenum Type;
        GLint Count;
        GLboolean Normalized;
        GLsizei Size;
    };

    class VertexBufferLayout
    {
    public:
        void PushElement(VertexBufferElement vbe)
        {
            m_Vbes.push_back(vbe);
        }

        inline const std::vector<VertexBufferElement>& GetVBEs() const { return m_Vbes; }

    private:
        std::vector<VertexBufferElement> m_Vbes;
    };

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const VertexBuffer& vb, const VertexBufferLayout& layout);
    private:
        GLuint m_ID;
        GLsizei m_Stride;
    };

}
