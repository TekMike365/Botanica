#pragma once

#include <cstdint>

#include "Core/Renderer/VertexArray.h"

namespace Botanica::OpenGL
{
    class VertexArray : public Botanica::VertexArray
    {
    public:
        VertexArray(const VertexBuffer &vb, const IndexBuffer &ib);
        ~VertexArray();

        void Bind() const override;
        void Unbind() const override;
    
    private:
        uint32_t m_ID;
    };
}
