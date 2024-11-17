#pragma once

#include <cstdint>

#include "Core/Renderer/VertexArray.h"

namespace Botanica::Renderer::OpenGL
{
    class VertexArray : public Renderer::VertexArray
    {
    public:
        VertexArray(BufferSPtr vb, BufferSPtr ib);
        ~VertexArray();

        void Bind() const override;
        void Unbind() const override;

        virtual inline BufferSPtr GetVertexBuffer() const override { return m_VertexBuffer; }
        virtual inline BufferSPtr GetIndexBuffer() const override { return m_IndexBuffer; }

    private:
        uint32_t m_ID;
        BufferSPtr m_VertexBuffer;
        BufferSPtr m_IndexBuffer;
    };
}
