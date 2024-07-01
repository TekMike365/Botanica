#include "Shader.h"

#include "Core.h"

namespace Botanica
{

    Shader::Shader(const std::string &filepath, GLuint type)
        :m_ID(0)
    {
        std::ifstream ifs(filepath, std::ifstream::in);
        if (!ifs.good())
            BT_ERROR("File \"{}\" couldn't be opened.", filepath);

        std::stringstream buffer;
        buffer << ifs.rdbuf();
        m_Source = buffer.str();
        const char* source = m_Source.c_str();

        m_ID = glCreateShader(type);
        glShaderSource(m_ID, 1, &source, NULL);
        glCompileShader(m_ID);
    }

    Shader::~Shader()
    {
        glDeleteShader(m_ID);
    }

}
