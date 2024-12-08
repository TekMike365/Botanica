#include "pch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace Renderer
{
    GLenum GetGLTarget(BufferType type) {
        switch (type)
        {
        case BufferType::None: return GL_SHADER_STORAGE_BUFFER;
        case BufferType::Vertex: return GL_ARRAY_BUFFER;
        case BufferType::Index: return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::ShaderStorage: return GL_SHADER_STORAGE_BUFFER;
        case BufferType::Uniform: return GL_UNIFORM_BUFFER;
        }
        BT_ASSERT(false, "Invalid shader type!");
        return 0;
    }

    GLenum GetGLUsage(BufferUsage usage) {
        switch (usage)
        {
        case BufferUsage::StreamDraw: return GL_STREAM_DRAW;
        case BufferUsage::StreamRead: return GL_STREAM_READ;
        case BufferUsage::StreamCopy: return GL_STREAM_COPY;

        case BufferUsage::StaticDraw: return GL_STATIC_DRAW;
        case BufferUsage::StaticRead: return GL_STATIC_READ;
        case BufferUsage::StaticCopy: return GL_STATIC_COPY;

        case BufferUsage::DynamicDraw: return GL_DYNAMIC_DRAW;
        case BufferUsage::DynamicRead: return GL_DYNAMIC_READ;
        case BufferUsage::DynamicCopy: return GL_DYNAMIC_COPY;
        }
        BT_ASSERT(false, "Invalid shader usage!");
        return 0;
    }

    Buffer::Buffer(size_t size, const void *data, BufferUsage usage)
    {
        glGenBuffers(1, &m_ID);
        Bind(BufferType::ShaderStorage);
        glBufferData(GetGLTarget(BufferType::ShaderStorage), size, data, GetGLUsage(usage));
        Unbind(BufferType::ShaderStorage);
    }

    Buffer::~Buffer()
    {
        glDeleteBuffers(1, &m_ID);
    }
    
    void Buffer::Bind(BufferType type) const
    {
        glBindBuffer(GetGLTarget(type), m_ID);
    }
    
    void Buffer::Unbind(BufferType type) const
    {
        glBindBuffer(GetGLTarget(type), 0);
    }
    
    void Buffer::UploadData(size_t start, size_t size, const void *data) const
    {
        Bind(BufferType::ShaderStorage);
        glBufferSubData(GetGLTarget(BufferType::ShaderStorage), start, size, data);
        Unbind(BufferType::ShaderStorage);
    }
}