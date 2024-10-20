#include "RenderCommand.h"

#include "Core/Platform/OpenGL/RendererAPI.h"

namespace Botanica
{
    RendererAPI *RenderCommand::s_Renderer = RendererAPI::Create();

    void RenderCommand::DrawIndexed()
    {
        s_Renderer->DrawIndexed();
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