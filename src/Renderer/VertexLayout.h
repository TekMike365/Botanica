#pragma once

#include "btpch.h"
#include <glad/glad.h>

namespace Botanica
{

    struct VertexElement
    {
        GLenum Type;
        GLint Count;
        GLboolean Normalized;
        GLsizei Size;
    };

    class VertexLayout
    {
    public:
        void PushElement(VertexElement vbe)
        {
            m_Vbes.push_back(vbe);
            m_Stride += vbe.Size;
        }

        inline GLsizei GetStride() const { return m_Stride; }
        inline const std::vector<VertexElement>& GetVBEs() const { return m_Vbes; }
    private:
        GLsizei m_Stride = 0;
        std::vector<VertexElement> m_Vbes;
    };

}
