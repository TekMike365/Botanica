#include "Shader.h"

#include <glad/glad.h>

#include "Core.h"
#include "Log.h"

namespace Botanica::OpenGL
{
    void LogShaderCompileError(uint32_t id)
    {
        GLint compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_TRUE)
            return;

        BT_CORE_ERROR("Shader couldn't be compiled.");

        GLint messageLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

        if (messageLength <= 0.0)
        {
            BT_CORE_WARN("No information provided.");
            return;
        }

        GLsizei log_length = 0;
        GLchar message[messageLength];
        glGetShaderInfoLog(id, messageLength, &log_length, message);

        BT_CORE_ERROR("Information:\n{}", std::string(message));
    }

    void LogProgramLinkError(uint32_t id)
    {
        GLint linked;
        glGetShaderiv(id, GL_LINK_STATUS, &linked);
        if (linked == GL_TRUE)
            return;

        BT_CORE_ERROR("Program couldn't be linked.");

        GLint messageLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

        if (messageLength <= 0.0)
        {
            BT_CORE_WARN("No information provided.");
            return;
        }

        GLsizei log_length = 0;
        GLchar message[messageLength];
        glGetShaderInfoLog(id, messageLength, &log_length, message);

        BT_CORE_ERROR("Information:\n{}", std::string(message));
    }

    Shader::Shader()
    {
        m_ID = glCreateProgram();
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
        glUseProgram(0);
    }

    void Shader::PushSource(ShaderSourceType type, const std::string &source)
    {
        uint32_t sourceID;
        switch (type)
        {
        case ShaderSourceType::Vertex:
            sourceID = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderSourceType::Geometry:
            sourceID = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case ShaderSourceType::Fragment:
            sourceID = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderSourceType::Compute:
            sourceID = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            BT_CORE_ASSERT(false, "Unknown shader source type!")
            break;
        }
        const char *src = source.c_str();
        glShaderSource(sourceID, 1, &src, NULL);
        glCompileShader(sourceID);
        LogShaderCompileError(sourceID);

        glAttachShader(m_ID, sourceID);
        glDeleteShader(sourceID);

        glLinkProgram(m_ID);
        LogProgramLinkError(m_ID);
    }
}