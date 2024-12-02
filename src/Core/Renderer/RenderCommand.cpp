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

    void RenderCommand::SetShaderUniforms(std::initializer_list<Uniform> uniforms)
    {
        s_Renderer->SetShaderUniforms(uniforms);
    }

    void RenderCommand::SetShaderUniformBuffers(const std::vector<UploadBuffer> &uploadBuffers)
    {
        s_Renderer->SetShaderUploadBuffers(uploadBuffers);
    }

    void RenderCommand::SetShaderUniformBuffers(std::initializer_list<UploadBuffer> uploadBuffers)
    {
        s_Renderer->SetShaderUploadBuffers(uploadBuffers);
    }

    void RenderCommand::ClearScreen()
    {
        s_Renderer->ClearScreen();
    }

    void RenderCommand::DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z)
    {
        s_Renderer->DispatchCompute(groups_x, groups_y, groups_z);
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
