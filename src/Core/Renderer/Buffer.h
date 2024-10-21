#pragma once

#include <cstdint>
#include <vector>
#include <string>

namespace Botanica
{
    enum class ShaderDataType
    {
        None = 0,
        Float,
        Float2,
        Float3,
        Float4,
        Int,
        Int2,
        Int3,
        Int4,
        Bool
    };

    inline uint32_t ShaderDataTypeSize(ShaderDataType type)
    {
        switch (type)
        {
        case ShaderDataType::Float:
            return 4;
        case ShaderDataType::Float2:
            return 4 * 2;
        case ShaderDataType::Float3:
            return 4 * 3;
        case ShaderDataType::Float4:
            return 4 * 4;
        case ShaderDataType::Int:
            return 4;
        case ShaderDataType::Int2:
            return 4 * 2;
        case ShaderDataType::Int3:
            return 4 * 3;
        case ShaderDataType::Int4:
            return 4 * 4;
        case ShaderDataType::Bool:
            return 1;
        }

        BT_CORE_ASSERT(false, "Unknown SaderDataType.");
        return 0;
    }

    struct BufferElement
    {
        std::string Name;
        ShaderDataType Type;
        bool Normalized;
        uint32_t Size;
        uint32_t Offset;

        BufferElement(ShaderDataType type, const std::string &name, bool normalized)
            : Name(name), Type(type), Normalized(normalized), Size(ShaderDataTypeSize(type)), Offset(0)
        {
        }

        uint32_t GetElementCount() const
        {
            switch (Type)
            {
            case ShaderDataType::Bool:
            case ShaderDataType::Int:
            case ShaderDataType::Float:
                return 1;
            case ShaderDataType::Int2:
            case ShaderDataType::Float2:
                return 2;
            case ShaderDataType::Int3:
            case ShaderDataType::Float3:
                return 3;
            case ShaderDataType::Int4:
            case ShaderDataType::Float4:
                return 4;
            }

            BT_CORE_ASSERT(false, "Unknown SaderDataType.");
            return 0;
        }
    };

    class BufferLayout
    {
    public:
        void PushElement(const BufferElement &element)
        {
            m_Elements.push_back(element);
            m_Elements.back().Offset = m_Stride;
            m_Stride += element.Size;
        }

        inline uint32_t GetStride() const { return m_Stride; }
        inline const std::vector<BufferElement> &GetElements() const { return m_Elements; }

        std::vector<BufferElement>::iterator begin() { return m_Elements.begin(); }
        std::vector<BufferElement>::iterator end() { return m_Elements.end(); }
        std::vector<BufferElement>::const_iterator begin() const { return m_Elements.begin(); }
        std::vector<BufferElement>::const_iterator end() const { return m_Elements.end(); }

    private:
        std::vector<BufferElement> m_Elements;
        uint32_t m_Stride = 0;
    };

    class VertexBuffer
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void SetLayout(const BufferLayout &layout) = 0;
        virtual const BufferLayout &GetLayout() const = 0;

        static VertexBuffer *Create(uint32_t size, const void *data);
    };

    class IndexBuffer
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual uint32_t GetCount() const = 0;

        static IndexBuffer *Create(uint32_t count, const uint32_t *data);
    };
}