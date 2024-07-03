#pragma once

#include "btpch.h"
#include <glad/glad.h>

namespace Botanica
{

    class Shader
    {
    public:
        Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;
    private:
        std::string* LoadFile(const std::string& filepath);
    private:
        GLuint m_ID;
    };

}
