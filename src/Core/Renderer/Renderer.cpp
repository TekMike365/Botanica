#include "Renderer.h"

#include <memory>

#include "RenderCommand.h"

namespace Botanica::Renderer
{
    std::shared_ptr<Camera> g_Camera;

    void BeginScene(std::shared_ptr<Camera> camera)
    {
        g_Camera = camera;
    }

    void Submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray)
    {
        shader->Bind();
        vertexArray->Bind();
        shader->UploadUniform("uVP", g_Camera->GetVPMat());
        RenderCommand::DrawIndexed(vertexArray);
    }
    
    void EndScene()
    {
    }
}