#include "btpch.h"
#include "RendererAPI.h"

#include <glad/glad.h>

namespace Botanica
{
    RendererAPI *RendererAPI::Create()
    {
        return new OpenGL::RendererAPI();
    }
}

namespace Botanica::OpenGL
{
    RendererAPI::RendererAPI()
    {
    }

    RendererAPI::~RendererAPI()
    {
    }

    void RendererAPI::DrawIndexed(std::shared_ptr<VertexArray> vertexArray) const
    {
        //TODO: refactor
        glDrawElements(GL_TRIANGLES, vertexArray->GetIndexBuffer()->GetCount(), GL_UNSIGNED_INT, 0);
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