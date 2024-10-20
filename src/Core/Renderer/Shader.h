#pragma once

#include <string>

namespace Botanica
{
    enum class ShaderType
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

        virtual void AddSource(ShaderType type, const std::string &source) = 0;

        static Shader *Create();
    };
}
