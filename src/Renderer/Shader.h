#pragma once

#include <string>
#include <initializer_list>
#include <unordered_map>

#include "Buffer.h"

namespace Renderer
{
    enum class ShaderSourceType
    {
        None = 0,
        Vertex,
        Fragment,
        Geometry,
        Compute
    };

    enum class UniformType
    {
        None = 0,
        Mat4, Mat3, Mat2,
        Float, Float2, Float3, Float4,
        Int, Int2, Int3, Int4,
        UInt, UInt2, UInt3, UInt4,
        FloatVec, Float2Vec, Float3Vec, Float4Vec,
        IntVec, Int2Vec, Int3Vec, Int4Vec,
        UIntVec, UInt2Vec, UInt3Vec, UInt4Vec,
    };

    class ShaderSource
    {
        friend class Shader;
    public:
        ShaderSource(ShaderSourceType type, const std::string &source);
        ~ShaderSource();

        inline ShaderSourceType GetType() const { return m_Type; }

    private:
        ShaderSourceType m_Type;
        uint32_t m_ID;
    };

    class Shader
    {
    public:
        Shader(std::initializer_list<ShaderSource *> sources);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void UploadUniform(UniformType type, const std::string &name, void *data, size_t count = 1);
        void UploadBuffer(BufferType type, const std::string &name, Buffer *buffer);

    private:
        void StashUniformLocations();
        void StashUniformBufferLocations();
        void StashShaderStorageLocations();

    private:
        uint32_t m_ID;
        std::unordered_map<std::string, int> m_LocationMap;
    };
}