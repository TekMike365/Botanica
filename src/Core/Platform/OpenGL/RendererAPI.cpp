#include "btpch.h"
#include "RendererAPI.h"

#include <glad/glad.h>

namespace Botanica::Renderer
{
    RendererAPI *RendererAPI::Create()
    {
        return new OpenGL::RendererAPI();
    }
}

namespace Botanica::Renderer::OpenGL
{
    void RendererAPI::SetClearColor(glm::vec4 color) const
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void OpenGL::RendererAPI::SetRenderState(const RenderState &state)
    {
        m_RenderState = state;
        state.ShaderPtr->Bind();
        state.VertexArrayPtr->Bind();
    }

    void OpenGL::RendererAPI::SetShaderUniforms(const std::vector<Uniform> &uniforms) const
    {
        for (auto &uniform : uniforms)
            m_RenderState.ShaderPtr->UploadUniform(uniform);
    }

    void RendererAPI::DrawIndexed(size_t count, size_t offset) const
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void*)offset);
    }

    void RendererAPI::ClearScreen() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}