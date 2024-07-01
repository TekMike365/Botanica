#pragma once

#include "btpch.h"
#include <glad/glad.h>

#include "VertexLayout.h"
#include "VertexBuffer.h"

namespace Botanica
{

    class VertexArray
    {
    public:
        VertexArray();
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        void AddVertexBuffer(const VertexBuffer& vb, const VertexLayout& layout);
    private:
        GLuint m_ID;
    };

}
