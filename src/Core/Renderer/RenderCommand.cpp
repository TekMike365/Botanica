#include "RenderCommand.h"

#include "Core/Platform/OpenGL/RendererAPI.h"

namespace Botanica::Renderer
{
    RendererAPI *RenderCommand::s_Renderer = RendererAPI::Create();

    void RenderCommand::SetRenderState(const RenderState &state)
    {
        s_Renderer->SetRenderState(state);
    }

    void RenderCommand::SetShaderUniforms(const std::vector<Uniform> &uniforms)
    {
        s_Renderer->SetShaderUniforms(uniforms);
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
        s_Renderer->Execute();
    }

    bool RenderCommand::IsRendering()
    {
        return s_Renderer->IsRendering();
    }
}
