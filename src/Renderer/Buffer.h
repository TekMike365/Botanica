#pragma once

#include <cstdint>

#include "Utils.h"
#include "Renderer.h"

namespace Renderer
{
    enum class BufferType
    {
        None = 0,
        Vertex,
        Index,
        Shader,
        Uniform
    };

    enum class BufferUsage
    {
        None = 0,
        StreamDraw,
        StreamRead,
        StreamCopy,
        StaticDraw,
        StaticRead,
        StaticCopy,
        DynamicDraw,
        DynamicRead,
        DynamicCopy
    };

    struct BufferLayout
    {
    public:
        struct DataType
        {
            ShaderDataType Type;
            bool Normalized;
            size_t Offset;

            DataType(ShaderDataType type, bool normalized = false)
                : Type(type), Normalized(normalized), Offset(0) {}
        };

    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<DataType> layout)
        {
            for (const auto &e : layout)
            {
                m_Layout.push_back(e);
                m_Layout.back().Offset = m_Stride;
                m_Stride += GetTypeSize(e.Type);
            }
        }

        inline uint32_t GetStride() const { return m_Stride; }

        inline std::vector<DataType>::const_iterator begin() const { return m_Layout.begin(); }
        inline std::vector<DataType>::const_iterator end() const { return m_Layout.end(); }

    private:
        std::vector<DataType> m_Layout;
        uint32_t m_Stride = 0;
    };

    class Buffer
    {
    public:
        Buffer(size_t size, const void *data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);
        ~Buffer();

        inline void SetLayout(const BufferLayout &layout) { m_Layout = layout; }
        inline const BufferLayout &GetLayout() const { return m_Layout; }

        void Bind(BufferType type) const;
        void Unbind(BufferType type) const;

        void UploadData(size_t start, size_t size, const void *data) const;

        inline uint32_t GetID() const { return m_ID; }

    private:
        uint32_t m_ID;
        BufferLayout m_Layout;
    };
}