#include "Renderer.h"

#include <memory>

#include "RenderCommand.h"

namespace Botanica
{
    std::shared_ptr<Camera> Renderer::s_Camera;

    void Renderer::BeginScene(std::shared_ptr<Camera> camera)
    {
        s_Camera = camera;
    }

    void Renderer::Submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        shader->UploadUniform("uVP", s_Camera->GetVPMat());
        RenderCommand::DrawIndexed(vertexArray);
    }
    
    void Renderer::EndScene()
    {
    }
}