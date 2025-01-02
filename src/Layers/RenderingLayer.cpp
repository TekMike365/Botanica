#include "pch.h"
#include "RenderingLayer.h"

#include "shaders/Shaders.h"
#include "Input.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&RenderingLayer::x, this, std::placeholders::_1)

RenderingLayer::RenderingLayer(Camera &cam, World &world)
    : m_Camera(cam), m_World(world)
{
}

void RenderingLayer::OnAttach()
{
    using namespace Renderer;

    {
        glm::vec3 vertices[] = {
            m_VoxelScale * (glm::vec3(0.0f, 0.0f, 0.0f) - glm::vec3(0.5f, 0.5f, 0.5f)),
            m_VoxelScale * (glm::vec3(m_World.VoxelIDs.GetSize().x, 0.0f, 0.0f) - glm::vec3(0.5f, 0.5f, 0.5f)),
            m_VoxelScale * (glm::vec3(m_World.VoxelIDs.GetSize().x, 0.0f, m_World.VoxelIDs.GetSize().z) - glm::vec3(0.5f, 0.5f, 0.5f)),
            m_VoxelScale * (glm::vec3(0.0f, 0.0f, m_World.VoxelIDs.GetSize().z) - glm::vec3(0.5f, 0.5f, 0.5f)),

            m_VoxelScale * (glm::vec3(0.0f, m_World.VoxelIDs.GetSize().y, 0.0f) - glm::vec3(0.5f, 0.5f, 0.5f)),
            m_VoxelScale * (glm::vec3(m_World.VoxelIDs.GetSize().x, m_World.VoxelIDs.GetSize().y, 0.0f) - glm::vec3(0.5f, 0.5f, 0.5f)),
            m_VoxelScale * (glm::vec3(m_World.VoxelIDs.GetSize().x, m_World.VoxelIDs.GetSize().y, m_World.VoxelIDs.GetSize().z) - glm::vec3(0.5f, 0.5f, 0.5f)),
            m_VoxelScale * (glm::vec3(0.0f, m_World.VoxelIDs.GetSize().y, m_World.VoxelIDs.GetSize().z) - glm::vec3(0.5f, 0.5f, 0.5f))};
        uint32_t indices[] = {0, 1, 1, 2, 2, 3, 3, 0, 4, 5, 5, 6, 6, 7, 7, 4, 0, 4, 1, 5, 2, 6, 3, 7};

        BufferLayout vbl({{ShaderDataType::Float3}});
        auto vb = std::make_shared<Buffer>(sizeof(vertices), (const void *)vertices);
        vb->SetLayout(vbl);

        BufferLayout ibl({{ShaderDataType::Float3}});
        auto ib = std::make_shared<Buffer>(sizeof(indices), (const void *)indices);
        ib->SetLayout(ibl);

        m_WorldBoundsVA = std::make_shared<VertexArray>(vb, ib);

        ShaderSource vertex(ShaderSourceType::Vertex, WorldBoundsVert_glsl);
        ShaderSource fragment(ShaderSourceType::Fragment, WorldBoundsFrag_glsl);
        m_WorldBoundsShader = std::shared_ptr<Shader>(new Shader({&vertex, &fragment}));
    }

    {
        size_t voxelCount = m_World.VoxelIDs.GetElementCount();
        size_t verticesPerVoxel = 24;
        size_t indicesPerVoxel = 36;

        m_VoxelBuffer = std::make_shared<Buffer>(voxelCount * sizeof(uint32_t), (const void *)m_World.VoxelIDs.GetData().data(), BufferUsage::StaticRead);

        std::vector<uint32_t> indices(voxelCount * indicesPerVoxel);
        for (int i = 0, j = 0; i < voxelCount * indicesPerVoxel;)
        {
            indices[i + 0] = j + 0;
            indices[i + 1] = j + 1;
            indices[i + 2] = j + 2;
            indices[i + 3] = j + 2;
            indices[i + 4] = j + 3;
            indices[i + 5] = j + 0;
            i += 6;
            j += 4;
        }

        BufferLayout vbl({{ShaderDataType::Float4}});
        auto vb = std::make_shared<Buffer>(voxelCount * verticesPerVoxel * vbl.GetStride(), nullptr, BufferUsage::DynamicDraw);
        vb->SetLayout(vbl);

        BufferLayout ibl({{ShaderDataType::UInt}});
        auto ib = std::make_shared<Buffer>(voxelCount * indicesPerVoxel * ibl.GetStride(), (const void *)indices.data());
        ib->SetLayout(ibl);

        m_VoxelVA = std::make_shared<VertexArray>(vb, ib);

        ShaderSource vertex(ShaderSourceType::Vertex, VoxelVert_glsl);
        ShaderSource fragment(ShaderSourceType::Fragment, VoxelFrag_glsl);
        m_VoxelRenderShader = std::shared_ptr<Shader>(new Shader({&vertex, &fragment}));

        ShaderSource voxelGen(ShaderSourceType::Compute, VoxelGen_glsl);
        m_VoxelGenCShader = std::shared_ptr<Shader>(new Shader({&voxelGen}));
    }
}

void RenderingLayer::OnUpdate(Timestep dt)
{
    using namespace Renderer;

    BT_SET_DLOG_MASK(LogMaskError | LogMaskInfo);

    if (m_World.VoxelIDs.DataChanged)
    {
        m_World.VoxelIDs.DataChanged = false;
        size_t voxelCount = m_World.VoxelIDs.GetElementCount();
        m_VoxelBuffer->UploadData(0, voxelCount * sizeof(uint32_t), m_World.VoxelIDs.GetData().data());

        m_VoxelGenCShader->Bind();
        m_VoxelGenCShader->UploadBuffer(BufferType::ShaderStorage, "ssboVoxels", m_VoxelBuffer.get());
        m_VoxelGenCShader->UploadBuffer(BufferType::ShaderStorage, "ssboVertices", m_VoxelVA->GetVertexBuffer().get());
        glm::uvec3 size(m_World.VoxelIDs.GetSize());
        m_VoxelGenCShader->UploadUniform(UniformType::UInt3, "uVoxelsSize", (const void *)&size);
        m_VoxelGenCShader->UploadUniform(UniformType::Float, "uVoxelScale", &m_VoxelScale);

        glm::uvec3 maxGroups = GetAPIInfo().MaxComputeWorkGroupCount;
        uint32_t groups_x = (uint32_t)(((float)m_World.VoxelIDs.GetElementCount() + 0.5f) / 32.0f);
        uint32_t groups_y = 1, groups_z = 1;

        if (groups_x > maxGroups.x)
        {
            groups_y = (uint32_t)(((float)groups_x + 0.5f) / (float)maxGroups.x);
            groups_x = maxGroups.x;
        }

        if (groups_y > maxGroups.y)
        {
            groups_z = (uint32_t)(((float)groups_y + 0.5f) / (float)maxGroups.y);
            groups_y = maxGroups.y;
        }

        if (groups_z > maxGroups.z)
            BT_ASSERT(false, "Too much invocations. (max: {})", maxGroups.x * maxGroups.y * maxGroups.z)

        DispatchCompute(groups_x, groups_y, groups_z);
        SetMemoryBarrier(MemoryBarrierShaderStorage);
    }

    SetClearColor(glm::vec4(0.5f, 0.7f, 0.8f, 1.0f));
    ClearScreen();

    m_VoxelVA->Bind();
    m_VoxelRenderShader->Bind();
    m_VoxelRenderShader->UploadUniform(UniformType::Mat4, "uVP", &m_Camera.GetVPMat());

    size_t indicesPerVoxel = 36;
    DrawIndexed(m_World.VoxelIDs.GetElementCount() * indicesPerVoxel, 0, m_DrawWireframe);

    m_WorldBoundsVA->Bind();
    m_WorldBoundsShader->Bind();
    m_WorldBoundsShader->UploadUniform(UniformType::Mat4, "uVP", &m_Camera.GetVPMat());
    DrawLinesIndexed(24);

    BT_SET_DLOG_MASK(LogMaskAll);
}

void RenderingLayer::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_CALLBACK(OnKeyReleased));
    dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_CALLBACK(OnWindowResized));
}

bool RenderingLayer::OnKeyReleased(KeyReleasedEvent &e)
{
    if (e.GetKey() == GLFW_KEY_F1)
    {
        m_DrawWireframe = !m_DrawWireframe;
        return true;
    }

    return false;
}

bool RenderingLayer::OnWindowResized(WindowResizeEvent &e)
{
    float aspect = (float)e.GetWidth() / (float)e.GetHeight();
    Transform camTrans = m_Camera.transform;
    m_Camera = Camera(m_Camera.GetFovDeg(), aspect, m_Camera.GetZNear(), m_Camera.GetZFar());
    m_Camera.transform = camTrans;
    return true;
}
