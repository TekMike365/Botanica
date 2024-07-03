#include "VertexLayout.h"

namespace Botanica
{

    void VertexLayout::PushElement(GLenum type, int count, bool normalized)
    {
        VertexElement& element = m_Elements.emplace_back();
        element.Count = count;
        element.Type = type;
        element.Normalized = normalized;
        element.Size = GetSizeOfType(type) * count;
        m_Stride += element.Size;
    }

    size_t VertexLayout::GetSizeOfType(GLenum type)
    {
        switch(type)
        {
            case GL_BYTE:
            case GL_UNSIGNED_BYTE:
                return sizeof(GLbyte);
            case GL_SHORT:
            case GL_UNSIGNED_SHORT:
                return sizeof(GLshort);
            case GL_INT_2_10_10_10_REV:
            case GL_INT:
            case GL_UNSIGNED_INT_2_10_10_10_REV:
            case GL_UNSIGNED_INT:
                return sizeof(GLint);
            case GL_FLOAT:
                return sizeof(GLfloat);
            case GL_DOUBLE:
                return sizeof(GLdouble);
            case GL_FIXED:
                return sizeof(GLfixed);
            case GL_HALF_FLOAT:
                return sizeof(GLhalf);
        }

        return 0;
    }

}
