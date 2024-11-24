#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

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
        None = 0, Mat4
    };

    struct Uniform
    {
        UniformType Type;
        const std::string &Name;
        const void *Data;
    };

    class Shader
    {
    protected:
        using ShaderSourceSPtrVec = std::vector<std::shared_ptr<ShaderSource>>;

    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniform(const Uniform& uniform) const = 0;

        inline void UploadUniform(const std::string &name, const glm::mat4 &mat) const {
            UploadUniform({UniformType::Mat4, name, glm::value_ptr(mat)});
        }

        static std::shared_ptr<Shader> Create(const ShaderSourceSPtrVec &sources);
    };
}
