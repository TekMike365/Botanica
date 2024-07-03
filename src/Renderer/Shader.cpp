#include "Shader.h"

#include "Core.h"
#include <glm/gtc/type_ptr.hpp>

namespace Botanica
{

    void LogShaderCompileError(GLuint id)
    {
        GLint compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_TRUE)
            return;

        BT_ERROR("Shader couldn't be compiled.");

        GLint messageLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

        if (messageLength <= 0.0)
        {
            BT_WARN("Shader has no info log.");
            return;
        }

        GLsizei log_length = 0;
        GLchar message[messageLength];
        glGetShaderInfoLog(id, messageLength, &log_length, message);

        BT_ERROR("Shader info log:\n{}", std::string(message));
    }

    Shader::Shader(const std::string &vertexFilepath, const std::string &fragmentFilepath)
    {
        auto vertexSource = std::make_unique<std::string>(*LoadFile(vertexFilepath));
        auto fragmentSource = std::make_unique<std::string>(*LoadFile(fragmentFilepath));

        const char* pVertexSource = vertexSource->c_str();
        const char* pFragmentSource = fragmentSource->c_str();

        GLuint vertex = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertex, 1, &pVertexSource, NULL);
        glCompileShader(vertex);
        LogShaderCompileError(vertex);

        GLuint fragment = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragment, 1, &pFragmentSource, NULL);
        glCompileShader(fragment);
        LogShaderCompileError(fragment);

        m_ID = glCreateProgram();
        glAttachShader(m_ID, vertex);
        glAttachShader(m_ID, fragment);
        glLinkProgram(m_ID);

        glDeleteShader(vertex);
        glDeleteShader(fragment);
    }

    Shader::~Shader()
    {
        glDeleteProgram(m_ID);
    }

    void Shader::Bind() const
    {
        glUseProgram(m_ID);
    }

    void Shader::Unbind() const
    {
        glUseProgram(m_ID);
    }

    void Shader::SetMat4(const std::string &name, glm::mat4 mat)
    {
        GLint loc = GetUniformLocation(name.c_str());
        glUniformMatrix4fv(loc, 1, GL_FALSE, glm::value_ptr(mat));
    }

    std::string* Shader::LoadFile(const std::string &filepath)
    {
        std::ifstream ifs(filepath, std::ifstream::in);
        if (!ifs.good())
            BT_ERROR("File \"{}\" couldn't be opened.", filepath);

        std::stringstream buffer;
        buffer << ifs.rdbuf();
        return new std::string(buffer.str());
    }

}
