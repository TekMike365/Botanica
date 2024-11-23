#include "CommandBuffer.h"

#include "Core/Platform/OpenGL/RendererAPI.h"

namespace Botanica::Renderer
{
    RendererAPI *CommandBuffer::s_Renderer = RendererAPI::Create();

    void CommandBuffer::SetShader(std::shared_ptr<Shader> shader)
    {
        shader->Bind();
    }

    void CommandBuffer::SetVertexArray(std::shared_ptr<VertexArray> va)
    {
        va->Bind();
    }

    void CommandBuffer::SetClearColor(glm::vec4 color)
    {
        s_Renderer->SetClearColor(color);
    }

    void CommandBuffer::DrawIndexed(size_t count, size_t offset)
    {
        s_Renderer->DrawIndexed(count, offset);
    }
    
    void CommandBuffer::ClearScreen()
    {
        s_Renderer->ClearScreen();
    }
}