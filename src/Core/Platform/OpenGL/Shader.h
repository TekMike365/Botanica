#pragma once
#include "btpch.h"

namespace Botanica::OpenGL
{
    enum class ShaderSourceType
    {
        Vertex,
        Geometry,
        Fragment,
        Compute
    };

    class Shader
    {
    public:
        Shader();
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void PushSource(ShaderSourceType type, const std::string &source);
        
    private:
        std::vector<uint32_t> m_SourceIDs;
        uint32_t m_ID;
    };
}