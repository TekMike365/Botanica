#pragma once

#include <cstdint>

#include "Core/Renderer/Buffer.h"

namespace Botanica::OpenGL
{
    class IndexBuffer : public Botanica::IndexBuffer
    {
    public:
        IndexBuffer(uint32_t count, const uint32_t *data);
        ~IndexBuffer();

        void Bind() const override;
        void Unbind() const override;

        inline uint32_t GetCount() const override { return m_Count; }

        virtual void UploadData(uint32_t offset, uint32_t size, const void *data) const override;

    private:
        uint32_t m_Count;
        uint32_t m_ID;
    };

    class VertexBuffer : public Botanica::VertexBuffer
    {
    public:
        VertexBuffer(uint32_t size, const void *data);
        ~VertexBuffer();

        void Bind() const override;
        void Unbind() const override;

        inline void SetLayout(const BufferLayout &layout) override { m_Layout = layout; }
        inline const BufferLayout &GetLayout() const override { return m_Layout; }

        virtual void UploadData(uint32_t offset, uint32_t size, const void *data) const override;

    private:
        BufferLayout m_Layout;
        uint32_t m_ID;
    };
}