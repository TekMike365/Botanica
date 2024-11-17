#include "btpch.h"
#include "Shader.h"

#include <glad/glad.h>
#include <glm/gtc/type_ptr.hpp>

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
    glGetProgramiv(id, GL_LINK_STATUS, &linked);
    if (linked == GL_TRUE)
        return;

    BT_CORE_ERROR("Program couldn't be linked.");

    GLint messageLength;
    glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);

    if (messageLength <= 0.0)
    {
        BT_CORE_WARN("No information provided.");
        return;
    }

    GLsizei log_length = 0;
    GLchar message[messageLength];
    glGetProgramInfoLog(id, messageLength, &log_length, message);

    BT_CORE_ERROR("Information:\n{}", std::string(message));
}

namespace Botanica::Renderer
{
    std::shared_ptr<ShaderSource> ShaderSource::Create(ShaderSourceType type, const std::string &source)
    {
        return std::make_shared<OpenGL::ShaderSource>(type, source);
    }

    std::shared_ptr<Shader> Shader::Create(const ShaderSourceSPtrVec &sources)
    {
        return std::make_shared<OpenGL::Shader>(sources);
    }
}

namespace Botanica::Renderer::OpenGL
{
    ShaderSource::ShaderSource(ShaderSourceType type, const std::string &source)
        :m_Type(type)
    {
        switch (type)
        {
        case ShaderSourceType::Vertex:
            m_ID = glCreateShader(GL_VERTEX_SHADER);
            break;
        case ShaderSourceType::Geometry:
            m_ID = glCreateShader(GL_GEOMETRY_SHADER);
            break;
        case ShaderSourceType::Fragment:
            m_ID = glCreateShader(GL_FRAGMENT_SHADER);
            break;
        case ShaderSourceType::Compute:
            m_ID = glCreateShader(GL_COMPUTE_SHADER);
            break;
        default:
            BT_CORE_ASSERT(false, "Unknown shader source type!")
            break;
        }
        const char *src = source.c_str();
        glShaderSource(m_ID, 1, &src, NULL);
        glCompileShader(m_ID);
        LogShaderCompileError(m_ID);
    }

    ShaderSource::~ShaderSource()
    {
        glDeleteShader(m_ID);
    }

    Shader::Shader(const ShaderSourceSPtrVec &sources)
    {
        m_ID = glCreateProgram();

        for (const auto s : sources)
            glAttachShader(m_ID, std::static_pointer_cast<ShaderSource>(s)->m_ID);

        glLinkProgram(m_ID);
        LogProgramLinkError(m_ID);

        int uniformCount;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &uniformCount);

        int maxNameLen = 0;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);

        GLsizei size;
        GLenum type;
        char name[maxNameLen];
        for (int i = 0; i < uniformCount; i++)
        {
            //! segfault v
            glGetActiveUniform(m_ID, i, maxNameLen, nullptr, &size, &type, (GLchar *)&name);
            m_UniformNameLocationMap[name] = glGetUniformLocation(m_ID, name);
        }
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

    void Shader::UploadUniform(const std::string &name, const glm::mat4 &mat) const
    {
        int location = m_UniformNameLocationMap.at(name);
        glUniformMatrix4fv(location, 1, GL_FALSE, glm::value_ptr(mat));
    }
}