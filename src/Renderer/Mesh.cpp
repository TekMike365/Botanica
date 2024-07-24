#include "Mesh.h"

#include <glad/glad.h>

namespace Botanica::Renderer
{

    Mesh::Mesh(const void * vertices, uint32_t verticesSize, const VertexLayout vertexLayout, const uint32_t * indices, uint32_t indexCount)
        :m_VB(vertices, verticesSize) , m_IB(indices, indexCount)
    {
        m_VA.AddVertexBuffer(m_VB, vertexLayout);
    }

    void Mesh::Render(const Shader &shader)
    {
        shader.Bind();
        m_VA.Bind();
        glDrawElements(GL_TRIANGLES, m_IB.GetCount(), GL_UNSIGNED_INT, 0);
    }

}
