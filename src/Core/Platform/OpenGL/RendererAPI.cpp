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

    void OpenGL::RendererAPI::SetShader(std::shared_ptr<Shader> shader) const
    {
        shader->Bind();
    }

    void OpenGL::RendererAPI::SetVertexArray(std::shared_ptr<VertexArray> va) const
    {
        va->Bind();
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