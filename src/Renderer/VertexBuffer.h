#pragma once

#include <glad/glad.h>

namespace Botanica::Renderer
{

    class VertexBuffer
    {
    public:
        VertexBuffer(const void* data, uint32_t size);
        ~VertexBuffer();

        void Bind() const;
        void Unbind() const;

    private:
        GLuint m_ID;
    };

}
