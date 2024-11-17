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
    RendererAPI::RendererAPI()
    {
    }

    RendererAPI::~RendererAPI()
    {
    }

    void RendererAPI::DrawIndexed(std::shared_ptr<VertexArray> vertexArray) const
    {
        glDrawElements(GL_TRIANGLES, vertexArray->IndexCount, GL_UNSIGNED_INT, 0);
    }

    void RendererAPI::SetClearColor(glm::vec4 color) const
    {
        glClearColor(color.r, color.g, color.b, color.a);
    }

    void RendererAPI::ClearScreen() const
    {
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
    }
}