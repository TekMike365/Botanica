#include "RenderCommand.h"

#include "Core/Platform/OpenGL/RendererAPI.h"

// ! All this buffer stuff belongs to RendererAPI
// ! Rename CommandBuffer to RenderCommand (more fitting name)

namespace Botanica::Renderer
{
    RendererAPI *RenderCommand::s_Renderer = RendererAPI::Create();

    void RenderCommand::SetShader(std::shared_ptr<Shader> shader)
    {
        s_Renderer->SetShader(shader);
    }

    void RenderCommand::SetVertexArray(std::shared_ptr<VertexArray> va)
    {
        s_Renderer->SetVertexArray(va);
    }

    void RenderCommand::SetClearColor(glm::vec4 color)
    {
        s_Renderer->SetClearColor(color);
    }
    
    void RenderCommand::ClearScreen()
    {
        s_Renderer->ClearScreen();
    }

    void RenderCommand::DrawIndexed(size_t count, size_t offset)
    {
        s_Renderer->DrawIndexed(count, offset);
    }

    void RenderCommand::Execute()
    {
    }
}
