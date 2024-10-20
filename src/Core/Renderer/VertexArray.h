#pragma once

#include "Buffer.h"

namespace Botanica
{
    class VertexArray
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static VertexArray *Create(const VertexBuffer &vb, const IndexBuffer &ib);
    };
}
