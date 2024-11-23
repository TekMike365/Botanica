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

    void RendererAPI::DrawIndexed(size_t count, size_t offset) const
    {
        glDrawElements(GL_TRIANGLES, count, GL_UNSIGNED_INT, (const void*)offset);
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