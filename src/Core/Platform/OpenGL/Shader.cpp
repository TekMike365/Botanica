#include "btpch.h"
#include "Shader.h"

#include <glad/glad.h>

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

        StoreUniformNameLocations();
        StoreUniformBufferNameLocations();
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

    void Shader::UploadUniform(const Uniform &uniform) const
    {
        int location = m_UniformNameLocationMap.at(uniform.Name);
        switch (uniform.Type)
        {
        case UniformType::Mat4: glUniformMatrix4fv(location, uniform.Count, GL_FALSE, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::Mat3: glUniformMatrix3fv(location, uniform.Count, GL_FALSE, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::Mat2: glUniformMatrix2fv(location, uniform.Count, GL_FALSE, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::FloatVec: glUniform1fv(location, uniform.Count, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::Float2Vec: glUniform1fv(location, uniform.Count, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::Float3Vec: glUniform1fv(location, uniform.Count, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::Float4Vec: glUniform1fv(location, uniform.Count, (const GLfloat*)uniform.Data.get()); return;
        case UniformType::IntVec: glUniform1iv(location, uniform.Count, (const GLint*)uniform.Data.get()); return;
        case UniformType::Int2Vec: glUniform1iv(location, uniform.Count, (const GLint*)uniform.Data.get()); return;
        case UniformType::Int3Vec: glUniform1iv(location, uniform.Count, (const GLint*)uniform.Data.get()); return;
        case UniformType::Int4Vec: glUniform1iv(location, uniform.Count, (const GLint*)uniform.Data.get()); return;
        case UniformType::UIntVec: glUniform1uiv(location, uniform.Count, (const GLuint*)uniform.Data.get()); return;
        case UniformType::UInt2Vec: glUniform1uiv(location, uniform.Count, (const GLuint*)uniform.Data.get()); return;
        case UniformType::UInt3Vec: glUniform1uiv(location, uniform.Count, (const GLuint*)uniform.Data.get()); return;
        case UniformType::UInt4Vec: glUniform1uiv(location, uniform.Count, (const GLuint*)uniform.Data.get()); return;
        case UniformType::Float: glUniform1f(location, *(GLfloat*)uniform.Data.get()); return;
        case UniformType::Float2: glUniform2f(location, *(GLfloat*)uniform.Data.get(), *((GLfloat*)uniform.Data.get() + 1)); return;
        case UniformType::Float3: glUniform3f(location, *(GLfloat*)uniform.Data.get(), *((GLfloat*)uniform.Data.get() + 1), *((GLfloat*)uniform.Data.get() + 2)); return;
        case UniformType::Float4: glUniform4f(location, *(GLfloat*)uniform.Data.get(), *((GLfloat*)uniform.Data.get() + 1), *((GLfloat*)uniform.Data.get() + 2), *((GLfloat*)uniform.Data.get() + 3)); return;
        case UniformType::Int: glUniform1i(location, *(GLint*)uniform.Data.get()); return;
        case UniformType::Int2: glUniform2i(location, *(GLint*)uniform.Data.get(), *((GLint*)uniform.Data.get() + 1)); return;
        case UniformType::Int3: glUniform3i(location, *(GLint*)uniform.Data.get(), *((GLint*)uniform.Data.get() + 1), *((GLint*)uniform.Data.get() + 2)); return;
        case UniformType::Int4: glUniform4i(location, *(GLint*)uniform.Data.get(), *((GLint*)uniform.Data.get() + 1), *((GLint*)uniform.Data.get() + 2), *((GLint*)uniform.Data.get() + 3)); return;
        case UniformType::UInt: glUniform1i(location, *(GLuint*)uniform.Data.get()); return;
        case UniformType::UInt2: glUniform2i(location, *(GLuint*)uniform.Data.get(), *((GLuint*)uniform.Data.get() + 1)); return;
        case UniformType::UInt3: glUniform3i(location, *(GLuint*)uniform.Data.get(), *((GLuint*)uniform.Data.get() + 1), *((GLuint*)uniform.Data.get() + 2)); return;
        case UniformType::UInt4: glUniform4i(location, *(GLuint*)uniform.Data.get(), *((GLuint*)uniform.Data.get() + 1), *((GLuint*)uniform.Data.get() + 2), *((GLuint*)uniform.Data.get() + 3)); return;
        }
    }

    void Shader::UploadUniformBuffer(const UniformBuffer &ub) const
    {
        int location = m_UniformBufferNameLocationMap.at(ub.Name);
        glUniformBlockBinding(m_ID, location, ub.Binding);
        auto buff = std::static_pointer_cast<Buffer>(ub.Buffer);
        glBindBufferBase(GL_UNIFORM_BUFFER, location, buff->GetID());
    }

    void Shader::StoreUniformNameLocations()
    {
        int count;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORMS, &count);

        int maxNameLen = 0;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_MAX_LENGTH, &maxNameLen);

        GLsizei size;
        char name[maxNameLen];
        for (int i = 0; i < count; i++)
        {
            glGetActiveUniformName(m_ID, i, maxNameLen, &size, name);
            m_UniformNameLocationMap[name] = glGetUniformLocation(m_ID, name);
        }
    }

    void Shader::StoreUniformBufferNameLocations()
    {
        int count;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_BLOCKS, &count);

        int maxNameLen = 0;
        glGetProgramiv(m_ID, GL_ACTIVE_UNIFORM_BLOCK_MAX_NAME_LENGTH, &maxNameLen);

        GLsizei size;
        char name[maxNameLen];
        for (int i = 0; i < count; i++)
        {
            glGetActiveUniformBlockName(m_ID, i, maxNameLen,&size, name);
            m_UniformNameLocationMap[name] = glGetUniformBlockIndex(m_ID, name); // no idea if i need to get the index again ¯\_(ツ)_/¯
        }
    }
}