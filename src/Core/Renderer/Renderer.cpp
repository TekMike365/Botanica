#include "Renderer.h"

#include "RenderCommand.h"

namespace Botanica
{
    void Renderer::BeginScene()
    {
    }

    void Renderer::Submit(const Shader &shader, const VertexArray &mesh)
    {
        shader.Bind();
        mesh.Bind();
        RenderCommand::DrawIndexed();
    }
    
    void Renderer::EndScene()
    {
    }
}