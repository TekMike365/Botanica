#include "RenderCommand.h"

#include "Core/Platform/OpenGL/RendererAPI.h"

namespace Botanica::Renderer
{
    RendererAPI *RenderCommand::s_Renderer = RendererAPI::Create();

    void RenderCommand::DrawIndexed(std::shared_ptr<VertexArray> vertexArray)
    {
        s_Renderer->DrawIndexed(vertexArray);
    }
    
    void RenderCommand::SetClearColor(glm::vec4 color)
    {
        s_Renderer->SetClearColor(color);
    }
    
    void RenderCommand::ClearScreen()
    {
        s_Renderer->ClearScreen();
    }
}