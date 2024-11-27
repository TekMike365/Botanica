#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "Buffer.h"

namespace Botanica::Renderer
{
    enum class ShaderSourceType
    {
        None = 0,
        Vertex,
        Fragment,
        Geometry,
        Compute
    };

    class ShaderSource
    {
    public:
        virtual ShaderSourceType GetType() const = 0;

        static std::shared_ptr<ShaderSource> Create(ShaderSourceType type, const std::string &source);
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

    struct Uniform
    {
        UniformType Type = UniformType::None;
        std::string Name = "";
        std::shared_ptr<void> Data = nullptr;
        size_t Count = 1;

        Uniform() = default;
        Uniform(UniformType type, const std::string &name, std::shared_ptr<void> data, size_t count = 1)
            : Type(type), Name(name), Data(data), Count(count) {}
    };

    class Shader
    {
    protected:
        using ShaderSourceSPtrVec = std::vector<std::shared_ptr<ShaderSource>>;

    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniform(const Uniform &uniform) const = 0;
        virtual void UploadUniformBuffer(const std::string &name, std::shared_ptr<Buffer> ub, uint32_t bindingPoint) const = 0;

        static std::shared_ptr<Shader> Create(const ShaderSourceSPtrVec &sources);
    };
}
