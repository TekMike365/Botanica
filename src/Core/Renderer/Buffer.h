#pragma once

#include <cstdint>

namespace Botanica
{
    class VertexBuffer
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static VertexBuffer *Create(uint32_t size, void* data);
    };

    class IndexBuffer
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        static IndexBuffer *Create(uint32_t size, uint32_t data);
    };
}