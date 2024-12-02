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
    void OpenGL::RendererAPI::SetRenderState(const RenderState &state)
    {
        if (state.ShaderPtr)
            state.ShaderPtr->Bind();
        if (state.VertexArrayPtr)
            state.VertexArrayPtr->Bind();
        glClearColor(state.ClearColor.r, state.ClearColor.g, state.ClearColor.b, state.ClearColor.a);
        m_Shader = state.ShaderPtr;
        m_VertexArray = state.VertexArrayPtr;
    }

    void OpenGL::RendererAPI::SetShaderUniforms(const std::vector<Uniform> &uniforms)
    {
        for (auto &e : uniforms)
            m_Shader->UploadUniform(e);
    }

    void OpenGL::RendererAPI::SetShaderUploadBuffers(const std::vector<UploadBuffer> &uploadBuffers)
    {
        for (auto &e : uploadBuffers)
            m_Shader->UploadUploadBuffer(e);
    }

    void OpenGL::RendererAPI::DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z)
    {
        glDispatchCompute(groups_x, groups_y, groups_z);
        // make sure writing to buffers has finished before read
        glMemoryBarrier(GL_UNIFORM_BARRIER_BIT);
    }

    void RendererAPI::DrawIndexed(size_t count, size_t offset)
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void *)offset);
    }

    void RendererAPI::ClearScreen()
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}