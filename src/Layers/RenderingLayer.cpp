#include "pch.h"
#include "RenderingLayer.h"

#include "shaders/Shaders.h"

RenderingLayer::RenderingLayer(Camera &cam, Vector3D<uint32_t> &world)
    : m_Camera(cam), m_World(world)
{
}

void RenderingLayer::OnAttach()
{
    using namespace Renderer;

    size_t voxelCount = m_World.GetElementCount();
    size_t verticesPerVoxel = 24;
    size_t indicesPerVoxel = 36;

    m_VoxelBuffer = std::make_shared<Buffer>(voxelCount * sizeof(uint32_t), (const void *)m_World.GetData().data(), BufferUsage::StaticRead);

    uint32_t indices[voxelCount * indicesPerVoxel];
    for (int i = 0; i < voxelCount * indicesPerVoxel;)
    {
        indices[i + 0] = i + 0;
        indices[i + 1] = i + 1;
        indices[i + 2] = i + 2;
        indices[i + 3] = i + 2;
        indices[i + 4] = i + 3;
        indices[i + 5] = i + 0;
        i += 6;
    }

    BufferLayout vbl({
        { ShaderDataType::Float4 }
    });
    auto vb = std::make_shared<Buffer>(voxelCount * verticesPerVoxel * vbl.GetStride(), nullptr, BufferUsage::StaticDraw);
    vb->SetLayout(vbl);

    BufferLayout ibl({
        { ShaderDataType::UInt }
    });
    auto ib = std::make_shared<Buffer>(voxelCount * indicesPerVoxel * ibl.GetStride(), (const void *)indices);
    ib->SetLayout(ibl);

    m_VA = std::make_shared<VertexArray>(vb, ib);

    ShaderSource vertex(ShaderSourceType::Vertex, Vertex_glsl);
    ShaderSource fragment(ShaderSourceType::Fragment, Fragment_glsl);
    m_RenderShader = std::shared_ptr<Shader>(new Shader({&vertex, &fragment}));

    ShaderSource voxelGen(ShaderSourceType::Compute, VoxelGen_glsl);
    m_VoxelGenCShader = std::shared_ptr<Shader>(new Shader({&voxelGen}));

    // m_VoxelBuffer->Bind(BufferType::ShaderStorage);
    // m_VA->GetVertexBuffer()->Bind(BufferType::ShaderStorage);
    m_VoxelGenCShader->Bind();
    m_VoxelGenCShader->UploadBuffer(BufferType::ShaderStorage, "ssboVoxels", m_VoxelBuffer.get());
    m_VoxelGenCShader->UploadBuffer(BufferType::ShaderStorage, "ssboVertices", m_VA->GetVertexBuffer().get());
    glm::uvec3 size(m_World.GetSize());
    // m_VoxelGenCShader->UploadUniform(UniformType::UInt3, "uVoxelsSize", (const void *)&size);

    uint32_t groups_x = m_World.GetElementCount() / 32;
    DispatchCompute(groups_x);
    SetMemoryBarrier(MemoryBarrierShaderStorage);
}

void RenderingLayer::OnUpdate(Timestep dt)
{
    using namespace Renderer;

    BT_SET_DLOG_MASK(LogMaskAll & ~LogMaskTrace);

    SetClearColor(glm::vec4(0.5f, 0.7f, 0.8f, 1.0f));
    ClearScreen();

    m_VA->Bind();
    m_RenderShader->Bind();
    m_RenderShader->UploadUniform(UniformType::Mat4, "uVP", &m_Camera.GetVPMat());

    size_t indicesPerVoxel = 36;
    // DrawIndexed(m_World.GetElementCount() * indicesPerVoxel, 0, true);
    DrawIndexed(indicesPerVoxel, 0, true);

    BT_SET_DLOG_MASK(LogMaskAll);
}
