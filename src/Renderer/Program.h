#pragma once

#include "btpch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Botanica
{

    class Program
    {
    public:
        Program();
        ~Program();

        void Bind() const;
        void Unbind() const;

        void AttachShader(const Shader& shader);
        void DetachShader(const Shader& shader);

        void Link() const;
    private:
        GLuint m_ID;
    };

}
