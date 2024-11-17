#pragma once

#include <string>
#include <memory>

#include <glm/glm.hpp>

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

    class Shader
    {
    protected:
        using ShaderSourceSPtrVec = std::vector<std::shared_ptr<ShaderSource>>;

    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void UploadUniform(const std::string &name, const glm::mat4 &mat) const = 0;

        static std::shared_ptr<Shader> Create(const ShaderSourceSPtrVec &sources);
    };
}
