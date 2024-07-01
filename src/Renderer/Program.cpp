#include "Program.h"

namespace Botanica
{

    Program::Program()
        :m_ID(0)
    {
        m_ID = glCreateProgram();
    }

    Program::~Program()
    {
        glDeleteProgram(m_ID);
    }

    void Program::Bind() const
    {
        glUseProgram(m_ID);
    }

    void Program::Unbind() const
    {
        glUseProgram(0);
    }

    void Program::AttachShader(const Shader &shader)
    {
        glAttachShader(m_ID, shader.GetID());
        Link();
    }

    void Program::DetachShader(const Shader &shader)
    {
        glDetachShader(m_ID, shader.GetID());
        Link();
    }

    void Program::Link() const
    {
        glLinkProgram(m_ID);
    }

}
