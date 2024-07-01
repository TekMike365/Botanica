#pragma once

#include "btpch.h"
#include <glad/glad.h>

namespace Botanica
{

    class Shader
    {
    public:
        Shader(const std::string& filepath, GLuint type);
        ~Shader();

        inline GLuint GetID() const { return m_ID; }
    private:
        std::string m_Source;
        GLuint m_ID;
    };

}
