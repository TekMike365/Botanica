#pragma once

#include <cstdint>

#include "Core/Renderer/VertexArray.h"

namespace Botanica::OpenGL
{
    class VertexArray : public Botanica::VertexArray
    {
    public:
        VertexArray(VertexBuffer *vb, IndexBuffer *ib);
        ~VertexArray();

        void Bind() const override;
        void Unbind() const override;

        inline std::shared_ptr<VertexBuffer> GetVertexBuffer() const override { return m_VertexBuffer; }
        inline std::shared_ptr<IndexBuffer> GetIndexBuffer() const override { return m_IndexBuffer; }

    private:
        uint32_t m_ID;
        std::shared_ptr<VertexBuffer> m_VertexBuffer;
        std::shared_ptr<IndexBuffer> m_IndexBuffer;
    };
}
