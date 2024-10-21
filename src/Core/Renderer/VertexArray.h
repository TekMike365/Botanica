#pragma once

#include <memory>

#include "Buffer.h"

namespace Botanica
{
    class VertexArray
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual std::shared_ptr<VertexBuffer> GetVertexBuffer() const = 0;
        virtual std::shared_ptr<IndexBuffer> GetIndexBuffer() const = 0;

        static VertexArray *Create(VertexBuffer *vb, IndexBuffer *ib);
    };
}
