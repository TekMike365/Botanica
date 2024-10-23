#pragma once

#include <string>
#include <glm/glm.hpp>

namespace Botanica
{
    enum class ShaderSourceType
    {
        None = 0,
        Vertex,
        Fragment,
        Geometry
    };

    class Shader
    {
    public:
        virtual void Bind() const = 0;
        virtual void Unbind() const = 0;

        virtual void AddSource(ShaderSourceType type, const std::string &source) = 0;

        virtual void UploadUniform(const std::string &name, const glm::mat4 &mat) const = 0;

        static Shader *Create();
    };
}
