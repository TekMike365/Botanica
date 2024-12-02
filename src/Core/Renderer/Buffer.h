#pragma once

#include <cstdint>

namespace Botanica::Renderer
{
    enum class ShaderDataType
    {
        None = 0, Bool,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4
    };

    inline uint8_t GetSubTypeCount(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Bool:
        case ShaderDataType::Int:
        case ShaderDataType::UInt:
        case ShaderDataType::Float:
            return 1;
        case ShaderDataType::Int2:
        case ShaderDataType::UInt2:
        case ShaderDataType::Float2:
            return 2;
        case ShaderDataType::Int3:
        case ShaderDataType::UInt3:
        case ShaderDataType::Float3:
            return 3;
        case ShaderDataType::Int4:
        case ShaderDataType::UInt4:
        case ShaderDataType::Float4:
            return 4;
        }

        BT_CORE_ASSERT(false, "Unknown SaderDataType.");
        return 0;
    }

    inline uint32_t GetTypeSize(ShaderDataType type)
    {
        if (type == ShaderDataType::Bool)
            return 1;
        
        return GetSubTypeCount(type) * 4;
    }

    enum class BufferType {
        None = 0, Vertex, Index, Shader, Uniform
    };

    enum class BufferUsage {
        None = 0,
        StreamDraw, StreamRead, StreamCopy,
        StaticDraw, StaticRead, StaticCopy,
        DynamicDraw, DynamicRead, DynamicCopy
    };

    struct BufferLayout
    {
    public:
        struct DataType {
            ShaderDataType Type;
            bool Normalized;
            size_t Offset;

            DataType(ShaderDataType type, bool normalized = false)
                :Type(type), Normalized(normalized), Offset(0) {}
        };

    public:
        BufferLayout() {}

        BufferLayout(std::initializer_list<DataType> layout){
            for (const auto& e : layout) {
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
        inline void SetLayout(const BufferLayout &layout) { m_Layout = layout; }
        inline const BufferLayout &GetLayout() const { return m_Layout; }

        static std::shared_ptr<Buffer> Create(size_t size = MAX_SIZE, const void* data = nullptr, BufferUsage usage = BufferUsage::StaticDraw);

    public:
        static const size_t MAX_SIZE = 1024;

    public:
        virtual void Bind(BufferType type) const = 0;
        virtual void Unbind(BufferType type) const = 0;

        virtual void UploadData(size_t start, size_t size, const void* data) const = 0;

    private:
        BufferLayout m_Layout;
    };
}