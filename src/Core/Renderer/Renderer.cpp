#include "Renderer.h"

#include <memory>

#include "RenderCommand.h"

namespace Botanica
{
    void Renderer::BeginScene()
    {
    }

    void Renderer::Submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        RenderCommand::DrawIndexed(vertexArray);
    }
    
    void Renderer::EndScene()
    {
    }
}