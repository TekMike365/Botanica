#include "btpch.h"
#include "MainLayer.h"

#include <glm/gtc/type_ptr.hpp>

#include "Core/Renderer/Renderer.h"
#include "Core/Renderer/RenderPipeline.h"
#include "Core/Renderer/RenderCommand.h"

#include "Core/Input.h"
#include "Core/Application.h"

namespace App
{
    using namespace Botanica;

    MainLayer::MainLayer()
        : Layer("Main Layer"), m_CameraController(new CameraController()), m_World(new World(glm::uvec3(8, 8, 8)))
    {
        m_ObjStack.PushLayer(m_World);
        m_ObjStack.PushLayer(m_CameraController);
        m_CameraController->GetCamera().transform.SetPosition(glm::vec3(0.0f, 0.0f, 10.0f));

        Setup();
    }

    MainLayer::~MainLayer()
    {
    }

    void MainLayer::OnAttach()
    {
        Renderer::RenderPipeline pipeline({std::bind(&MainLayer::RenderPass, this)});
        Renderer::SetRenderPipeline(pipeline);

        for (auto &e : m_ObjStack)
            e->OnAttach();
    }

    void MainLayer::OnUpdate(Timestep dt)
    {
        for (auto &e : m_ObjStack)
            e->OnUpdate(dt);
    }

    void MainLayer::RenderPass()
    {
        using namespace Renderer;

        if (m_World->DataUpdated)
        {
            m_World->DataUpdated = false;

            RenderCommand::SetRenderState({.ShaderPtr = m_ComputeShader});

            RenderCommand::SetShaderUniformBuffers({
                {UploadBufferType::ShaderStorageBuffer, "Voxels", m_VoxelBuffer},
                {UploadBufferType::ShaderStorageBuffer, "Vertices", m_VertexArray->GetVertexBuffer()}
            });

            RenderCommand::DispatchCompute();
        }

        RenderCommand::SetRenderState({.ShaderPtr = m_Shader,
                                       .VertexArrayPtr = m_VertexArray,
                                       .ClearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)});
        RenderCommand::ClearScreen();

        RenderCommand::SetShaderUniforms({
            {UniformType::Mat4, "uVP", std::make_shared<glm::mat4>(m_CameraController->GetCamera().GetVPMat())}
        });

        RenderCommand::DrawIndexed(36 * m_World->GetVoxels().size());
    }

    void MainLayer::Setup()
    {
        using namespace Renderer;

        const char *compute = R"(
            #version 430 core
            layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

            struct Vertex {
                vec3 Position;
                vec4 Color;
            };

            layout (std430, binding = 0) buffer Voxels { uint b_IDs[]; };
            layout (std430, binding = 1) buffer Vertices { Vertex b_Vertices[]; };

            shared uint s_VertIdx = 0;

            uint GetIndex(uint x, uint y, uint z)
            {
                return x + y * 8 + z * 8 * 8;
            }

            vec4 g_Colors[] = {
                vec4(0.0f, 0.0f, 0.0f, 1.0f),
                vec4(0.0f, 0.0f, 1.0f, 1.0f),
                vec4(0.0f, 1.0f, 0.0f, 1.0f),
                vec4(0.0f, 1.0f, 1.0f, 1.0f),
                vec4(1.0f, 0.0f, 0.0f, 1.0f),
                vec4(1.0f, 0.0f, 1.0f, 1.0f),
                vec4(1.0f, 1.0f, 0.0f, 1.0f),
                vec4(1.0f, 1.0f, 1.0f, 1.0f)
            };

            uint g_VertCount = 0;
            float g_Scale = 1.0f / 8.0f;
            void SetVertex(vec3 position, vec4 color)
            {
                uint idx = g_VertCount++;
                b_Vertices[idx].Position = g_Scale * (position + gl_GlobalInvocationID);
                b_Vertices[idx].Color = color;
            }

            void main()
            {
                uint idx = GetIndex(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_GlobalInvocationID.z);
                uint id = b_IDs[idx];

                //! ???
                //! ids are 1 but they're set to 0
                //! geometry gets messed up when I for example compare id with zero????
                //if (id == 0)
                //    return;

                vec4 color = g_Colors[id];

                g_VertCount = atomicAdd(s_VertIdx, 24);
                float scale = 1.0f / 8.0f;

                // front
                SetVertex(vec3( 0.5f, -0.5f, -0.5f), color);
                SetVertex(vec3( 0.5f,  0.5f, -0.5f), color);
                SetVertex(vec3(-0.5f,  0.5f, -0.5f), color);
                SetVertex(vec3(-0.5f, -0.5f, -0.5f), color);

                // back
                SetVertex(vec3( 0.5f, -0.5f,  0.5f), color);
                SetVertex(vec3( 0.5f,  0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f,  0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f, -0.5f,  0.5f), color);

                // bottom
                SetVertex(vec3( 0.5f, -0.5f, -0.5f), color);
                SetVertex(vec3( 0.5f, -0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f, -0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f, -0.5f, -0.5f), color);

                // top
                SetVertex(vec3( 0.5f,  0.5f, -0.5f), color);
                SetVertex(vec3( 0.5f,  0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f,  0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f,  0.5f, -0.5f), color);

                // left
                SetVertex(vec3(-0.5f,  0.5f, -0.5f), color);
                SetVertex(vec3(-0.5f, -0.5f, -0.5f), color);
                SetVertex(vec3(-0.5f, -0.5f,  0.5f), color);
                SetVertex(vec3(-0.5f,  0.5f,  0.5f), color);

                // right
                SetVertex(vec3( 0.5f,  0.5f, -0.5f), color);
                SetVertex(vec3( 0.5f, -0.5f, -0.5f), color);
                SetVertex(vec3( 0.5f, -0.5f,  0.5f), color);
                SetVertex(vec3( 0.5f,  0.5f,  0.5f), color);
            }
        )";

        const char *vert = R"(
            #version 430 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec4 aCol;

            uniform mat4 uVP;

            out vec4 vCol;

            void main()
            {
                gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
                vCol = aCol;
            }
        )";

        const char *frag = R"(
            #version 430 core

            in vec4 vCol;

            out vec4 fColor;

            void main()
            {
                fColor = vCol;
            }
        )";

        const size_t vertexCount = m_World->GetVoxels().size() * 6 * 4;
        const size_t indexCount = m_World->GetVoxels().size() * 6 * 2 * 3;

        uint32_t indices[indexCount];
        for (int i = 0, j = 0; i < indexCount; j += 4)
        {
            indices[i++] = j + 0;
            indices[i++] = j + 1;
            indices[i++] = j + 2;
            indices[i++] = j + 2;
            indices[i++] = j + 3;
            indices[i++] = j + 0;
        }

        std::shared_ptr<ShaderSource> cs = ShaderSource::Create(ShaderSourceType::Compute, compute);
        m_ComputeShader = Shader::Create({cs});

        std::shared_ptr<ShaderSource> vs = ShaderSource::Create(ShaderSourceType::Vertex, vert);
        std::shared_ptr<ShaderSource> fs = ShaderSource::Create(ShaderSourceType::Fragment, frag);
        m_Shader = Shader::Create({vs, fs});

        BufferLayout voxelBL({ShaderDataType::UInt});
        m_VoxelBuffer = Buffer::Create(m_World->GetVoxels().size() * voxelBL.GetStride(), m_World->GetVoxels().data());
        m_VoxelBuffer->SetLayout(voxelBL);

        BufferLayout vbl({
            ShaderDataType::Float4, // Position (vec3 is float4 in shader)
            ShaderDataType::Float4, // Color
        });
        std::shared_ptr<Buffer> vb = Buffer::Create(vertexCount * vbl.GetStride());
        vb->SetLayout(vbl);

        BufferLayout ibl({ShaderDataType::UInt});
        std::shared_ptr<Buffer> ib = Buffer::Create(indexCount * ibl.GetStride(), indices);
        ib->SetLayout(ibl);

        m_VertexArray = VertexArray::Create(vb, ib);
    }
}
