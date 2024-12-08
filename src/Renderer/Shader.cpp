#include "pch.h"
#include "Shader.h"

#include <glad/glad.h>

namespace Renderer
{
    void LogShaderCompileError(uint32_t id)
    {
        GLint compiled;
        glGetShaderiv(id, GL_COMPILE_STATUS, &compiled);
        if (compiled == GL_TRUE)
            return;

        BT_DLOG_ERROR("Shader couldn't be compiled.");

        GLint messageLength;
        glGetShaderiv(id, GL_INFO_LOG_LENGTH, &messageLength);

        if (messageLength <= 0.0)
        {
            BT_DLOG_WARN("No information provided.");
            return;
        }

        GLsizei log_length = 0;
        GLchar message[messageLength];
        glGetShaderInfoLog(id, messageLength, &log_length, message);

        BT_DLOG_ERROR("Information:\n{}", std::string(message));
    }

    void LogProgramLinkError(uint32_t id)
    {
        GLint linked;
        glGetProgramiv(id, GL_LINK_STATUS, &linked);
        if (linked == GL_TRUE)
            return;

        BT_DLOG_ERROR("Program couldn't be linked.");

        GLint messageLength;
        glGetProgramiv(id, GL_INFO_LOG_LENGTH, &messageLength);

        if (messageLength <= 0.0)
        {
            BT_DLOG_WARN("No information provided.");
            return;
        }

        GLsizei log_length = 0;
        GLchar message[messageLength];
        glGetProgramInfoLog(id, messageLength, &log_length, message);

        BT_DLOG_ERROR("Information:\n{}", std::string(message));
    }

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
            BT_ASSERT(false, "Unknown shader source type!")
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

    Shader::Shader(std::initializer_list<ShaderSource *> sources)
    {
        m_ID = glCreateProgram();

        for (ShaderSource* s : sources)
            glAttachShader(m_ID, s->m_ID);

        glLinkProgram(m_ID);
        LogProgramLinkError(m_ID);

        StashUniformLocations();
        StashUniformBufferLocations();
        StashShaderStorageLocations();
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

    void Shader::UploadUniform(UniformType type, const std::string &name, const void *data, size_t count)
    {
        int location = m_LocationMap.at(name);
        switch (type)
        {
        case UniformType::Mat4: glUniformMatrix4fv(location, count, GL_FALSE, (const GLfloat*)data); return;
        case UniformType::Mat3: glUniformMatrix3fv(location, count, GL_FALSE, (const GLfloat*)data); return;
        case UniformType::Mat2: glUniformMatrix2fv(location, count, GL_FALSE, (const GLfloat*)data); return;
        case UniformType::FloatVec: glUniform1fv(location, count, (const GLfloat*)data); return;
        case UniformType::Float2Vec: glUniform1fv(location, count, (const GLfloat*)data); return;
        case UniformType::Float3Vec: glUniform1fv(location, count, (const GLfloat*)data); return;
        case UniformType::Float4Vec: glUniform1fv(location, count, (const GLfloat*)data); return;
        case UniformType::IntVec: glUniform1iv(location, count, (const GLint*)data); return;
        case UniformType::Int2Vec: glUniform1iv(location, count, (const GLint*)data); return;
        case UniformType::Int3Vec: glUniform1iv(location, count, (const GLint*)data); return;
        case UniformType::Int4Vec: glUniform1iv(location, count, (const GLint*)data); return;
        case UniformType::UIntVec: glUniform1uiv(location, count, (const GLuint*)data); return;
        case UniformType::UInt2Vec: glUniform1uiv(location, count, (const GLuint*)data); return;
        case UniformType::UInt3Vec: glUniform1uiv(location, count, (const GLuint*)data); return;
        case UniformType::UInt4Vec: glUniform1uiv(location, count, (const GLuint*)data); return;
        case UniformType::Float: glUniform1f(location, *(GLfloat*)data); return;
        case UniformType::Float2: glUniform2f(location, *(GLfloat*)data, *((GLfloat*)data + 1)); return;
        case UniformType::Float3: glUniform3f(location, *(GLfloat*)data, *((GLfloat*)data + 1), *((GLfloat*)data + 2)); return;
        case UniformType::Float4: glUniform4f(location, *(GLfloat*)data, *((GLfloat*)data + 1), *((GLfloat*)data + 2), *((GLfloat*)data + 3)); return;
        case UniformType::Int: glUniform1i(location, *(GLint*)data); return;
        case UniformType::Int2: glUniform2i(location, *(GLint*)data, *((GLint*)data + 1)); return;
        case UniformType::Int3: glUniform3i(location, *(GLint*)data, *((GLint*)data + 1), *((GLint*)data + 2)); return;
        case UniformType::Int4: glUniform4i(location, *(GLint*)data, *((GLint*)data + 1), *((GLint*)data + 2), *((GLint*)data + 3)); return;
        case UniformType::UInt: glUniform1i(location, *(GLuint*)data); return;
        case UniformType::UInt2: glUniform2i(location, *(GLuint*)data, *((GLuint*)data + 1)); return;
        case UniformType::UInt3: glUniform3i(location, *(GLuint*)data, *((GLuint*)data + 1), *((GLuint*)data + 2)); return;
        case UniformType::UInt4: glUniform4i(location, *(GLuint*)data, *((GLuint*)data + 1), *((GLuint*)data + 2), *((GLuint*)data + 3)); return;
        }
    }

    void Shader::UploadBuffer(BufferType type, const std::string &name, Buffer *buffer)
    {
        int location = m_LocationMap.at(name);

        GLenum target = 0;
        switch (type)
        {
        case BufferType::Uniform:
            target = GL_UNIFORM_BUFFER;
            break;
        case BufferType::ShaderStorage:
            target = GL_SHADER_STORAGE_BUFFER;
            break;
        default:
            BT_ASSERT(false, "Unsupported Buffer Type;");
        }

        // auto bufferGL = std::static_pointer_cast<Buffer>(buffer.Buffer);
        glBindBufferBase(target, location, buffer->m_ID);
    }

    void Shader::StashUniformLocations()
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
            m_LocationMap[name] = glGetUniformLocation(m_ID, name);
        }
    }

    void Shader::StashUniformBufferLocations()
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
            m_LocationMap[name] = glGetUniformBlockIndex(m_ID, name);
        }
    }

    void Shader::StashShaderStorageLocations()
    {
        int count;
        glGetProgramInterfaceiv(m_ID, GL_SHADER_STORAGE_BLOCK, GL_ACTIVE_RESOURCES, &count);

        int maxNameLen = 0;
        glGetProgramInterfaceiv(m_ID, GL_SHADER_STORAGE_BLOCK, GL_MAX_NAME_LENGTH, &maxNameLen);

        GLsizei size;
        int32_t binding;
        GLenum props[1] = { GL_BUFFER_BINDING };
        char name[maxNameLen];
        for (int i = 0; i < count; i++)
        {
            glGetProgramResourceName(m_ID, GL_SHADER_STORAGE_BLOCK, i, maxNameLen, &size, name);
            glGetProgramResourceiv(m_ID, GL_SHADER_STORAGE_BLOCK, i, 1, props, sizeof(binding), nullptr, &binding);
            m_LocationMap[name] = binding;
        }
    }
}