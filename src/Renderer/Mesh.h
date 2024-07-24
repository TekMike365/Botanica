#pragma once

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

namespace Botanica::Renderer
{

    class Mesh
    {
    public:
        Mesh(const void* vertices, uint32_t verticesSize, const VertexLayout vertexLayout, const uint32_t* indices, uint32_t indexCount);

        void Render(const Shader& shader);
    private:
        VertexArray m_VA;
        VertexBuffer m_VB;
        IndexBuffer m_IB;
    };

}
