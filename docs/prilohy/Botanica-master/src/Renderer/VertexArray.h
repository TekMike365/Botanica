#pragma once

#include <cstdint>

#include "Buffer.h"

namespace Renderer
{
    class VertexArray 
    {
    public:
        VertexArray(std::shared_ptr<Buffer> vb, std::shared_ptr<Buffer> ib);
        ~VertexArray();

        void Bind() const;
        void Unbind() const;

        inline std::shared_ptr<Buffer> &GetVertexBuffer() { return m_VertexBuffer; }
        inline std::shared_ptr<Buffer> &GetIndexBuffer() { return m_IndexBuffer; }

    private:
        uint32_t m_ID;
        std::shared_ptr<Buffer> m_VertexBuffer;
        std::shared_ptr<Buffer> m_IndexBuffer;
    };
}
