#pragma once

#include <cstdint>

#include "Core/Renderer/Buffer.h"

namespace Botanica::Renderer::OpenGL
{
    class Buffer : public Renderer::Buffer
    {
    public:
        Buffer(size_t size, const  void* data, BufferUsage usage);
        ~Buffer();

        virtual void Bind(BufferType type) const override;
        virtual void Unbind(BufferType type) const override;

        virtual void UploadData(size_t start, size_t size, const  void* data) const override;
    
    private:
        uint32_t m_ID;
    };
}