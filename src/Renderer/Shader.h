#pragma once

#include "btpch.h"
#include <glad/glad.h>
#include <glm/glm.hpp>

namespace Botanica
{

    class Shader
    {
    public:
        Shader(const std::string& vertexFilepath, const std::string& fragmentFilepath);
        ~Shader();

        void Bind() const;
        void Unbind() const;

        void SetMat4(const std::string& name, glm::mat4 mat);

        inline GLint GetUniformLocation(const char* name) { return glGetUniformLocation(m_ID, name); }
    private:
        std::string* LoadFile(const std::string& filepath);
    private:
        GLuint m_ID;
    };

}
