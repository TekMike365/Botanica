#pragma once

#include <cstdint>

#include "Core/Renderer/Buffer.h"

namespace Botanica::OpenGL
{
    class Buffer : public Botanica::Buffer
    {
    public:
        Buffer(size_t size, void* data, BufferUsage usage);
        ~Buffer();

        virtual void Bind(BufferType type) const override;
        virtual void Unbind(BufferType type) const override;

        virtual void UploadData(size_t start, size_t size, void* data) const override;
    
    private:
        uint32_t m_ID;
    };
}