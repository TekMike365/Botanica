#pragma once

#include <memory>

#include "Buffer.h"

namespace Botanica::Renderer
{
    class VertexArray
    {
    protected:
        using BufferSPtr = std::shared_ptr<Buffer>;
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual inline BufferSPtr GetVertexBuffer() const = 0;
        virtual inline BufferSPtr GetIndexBuffer() const = 0;

        static std::shared_ptr<VertexArray> Create(BufferSPtr vb, BufferSPtr ib);
    
    public:
        uint32_t IndexCount = 0;
    };
}
