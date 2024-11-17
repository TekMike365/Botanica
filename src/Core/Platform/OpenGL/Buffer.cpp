#include "btpch.h"
#include "Buffer.h"

#include <glad/glad.h>

namespace Botanica::Renderer
{
    std::shared_ptr<Buffer> Buffer::Create(size_t size, void* data, BufferUsage usage)
    {
        return std::make_shared<OpenGL::Buffer>(size, data, usage);
    }
}

namespace Botanica::Renderer::OpenGL
{
    GLenum GetGLTarget(BufferType type) {
        switch (type)
        {
        case BufferType::None: return GL_SHADER_STORAGE_BUFFER;
        case BufferType::VertexBuffer: return GL_ARRAY_BUFFER;
        case BufferType::IndexBuffer: return GL_ELEMENT_ARRAY_BUFFER;
        case BufferType::ShaderBuffer: return GL_SHADER_STORAGE_BUFFER;
        }
        BT_CORE_ASSERT(false, "Invalid shader type!");
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
        BT_CORE_ASSERT(false, "Invalid shader usage!");
        return 0;
    }

    Buffer::Buffer(size_t size, void *data, BufferUsage usage)
    {
        glGenBuffers(1, &m_ID);
        Bind(BufferType::ShaderBuffer);
        glBufferData(GetGLTarget(BufferType::ShaderBuffer), size, data, GetGLUsage(usage));
        Unbind(BufferType::ShaderBuffer);
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
    
    void Buffer::UploadData(size_t start, size_t size, void *data) const
    {
        Bind(BufferType::ShaderBuffer);
        glBufferSubData(GetGLTarget(BufferType::ShaderBuffer), start, size, data);
        Unbind(BufferType::ShaderBuffer);
    }
}
