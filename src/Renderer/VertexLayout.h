#pragma once

#include "btpch.h"
#include <glad/glad.h>

namespace Botanica::Renderer
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
        void PushElement(GLenum type, int count, bool normalized);
    
        inline GLsizei GetStride() const { return m_Stride; }
        inline const std::vector<VertexElement>& GetElements() const { return m_Elements; }
    private:
        size_t GetSizeOfType(GLenum type);
        GLsizei m_Stride = 0;
        std::vector<VertexElement> m_Elements;
    };

}
