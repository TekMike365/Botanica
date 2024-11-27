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
        : Layer("TestLayer"), m_CameraController(new CameraController()), m_World(new World())
    {
        m_ObjStack.PushLayer(m_World);
        m_ObjStack.PushLayer(m_CameraController);

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

        RenderCommand::SetRenderState({.ClearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)});
        RenderCommand::ClearScreen();

        RenderCommand::SetRenderState({.ShaderPtr = m_ComputeShader});

        std::vector<UniformBuffer> uniformBuffers;
        uniformBuffers.emplace_back("Voxels", m_VoxelBuffer);
        uniformBuffers.emplace_back("Vertices", m_VertexArray->GetVertexBuffer());
        RenderCommand::SetShaderUniformBuffers(uniformBuffers);

        RenderCommand::DispatchCompute();

        RenderCommand::SetRenderState({.ShaderPtr = m_Shader,
                                       .VertexArrayPtr = m_VertexArray});

        std::vector<Uniform> uniforms;
        uniforms.emplace_back(UniformType::Mat4, "uVP", std::make_shared<glm::mat4>(m_CameraController->GetCamera().GetVPMat()));
        RenderCommand::SetShaderUniforms(uniforms);

        RenderCommand::DrawIndexed(m_VertexArray->IndexCount, 0);
    }

    void MainLayer::Setup()
    {
        using namespace Renderer;

        const char *compute = R"(
            #version 430 core
            layout (local_size_x = 8, local_size_y = 8, local_size_z = 8) in;

            layout (std430, binding = 0) buffer Voxels { uint ids[]; };
            layout (std430, binding = 1) buffer Vertices { vec3 vertices[]; };

            shared uint vertIndex = 0;

            uint GetIndex(uint x, uint y, uint z)
            {
                return x + y * 8 + z * 8 * 8;
            }

            void main()
            {
                uint index = GetIndex(gl_GlobalInvocationID.x, gl_GlobalInvocationID.y, gl_GlobalInvocationID.z);

                uint vertidx = vertIndex;
                vertIndex += 6 * 4;
                float scale = 1.0f;

                // top
                vertices[vertidx]     = scale * (vec3( 0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 1] = scale * (vec3(-0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 2] = scale * (vec3(-0.5f,  0.5f, -0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 3] = scale * (vec3( 0.5f,  0.5f, -0.5f) + gl_GlobalInvocationID);

                // bottom
                vertices[vertidx + 4] = scale * (vec3( 0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 5] = scale * (vec3(-0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 6] = scale * (vec3( 0.5f, -0.5f, -0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 7] = scale * (vec3(-0.5f, -0.5f, -0.5f) + gl_GlobalInvocationID);

                // left
                vertices[vertidx +  8] = scale * (vec3(-0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx +  9] = scale * (vec3(-0.5f,  0.5f, -0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 10] = scale * (vec3(-0.5f, -0.5f, -0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 11] = scale * (vec3(-0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);

                // right
                vertices[vertidx + 12] = scale * (vec3( 0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 13] = scale * (vec3( 0.5f,  0.5f, -0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 14] = scale * (vec3( 0.5f, -0.5f, -0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 15] = scale * (vec3( 0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);

                // back
                vertices[vertidx + 16] = scale * (vec3( 0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 17] = scale * (vec3(-0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 18] = scale * (vec3(-0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 19] = scale * (vec3( 0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);

                // front
                vertices[vertidx + 20] = scale * (vec3( 0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 21] = scale * (vec3(-0.5f,  0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 22] = scale * (vec3(-0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);
                vertices[vertidx + 23] = scale * (vec3( 0.5f, -0.5f,  0.5f) + gl_GlobalInvocationID);
            }
        )";

        const char *vert = R"(
            #version 430 core
            layout (location = 0) in vec3 aPos;

            uniform mat4 uVP;

            void main()
            {
                gl_Position = uVP * vec4(aPos, 1.0f);
            }
        )";

        const char *frag = R"(
            #version 430 core

            out vec4 fColor;

            void main()
            {
                fColor = vec4(1.0f, 0.0f, 1.0f, 1.0f);
            }
        )";

        uint32_t indices[m_World->GetVoxels().size()];
        for (int i = 0; i < m_World->GetVoxels().size(); i++)
            indices[i] = i;

        std::shared_ptr<ShaderSource> computeSrc = ShaderSource::Create(ShaderSourceType::Compute, compute);
        m_ComputeShader = Shader::Create({computeSrc});

        std::shared_ptr<ShaderSource> vertSrc = ShaderSource::Create(ShaderSourceType::Vertex, vert);
        std::shared_ptr<ShaderSource> fragSrc = ShaderSource::Create(ShaderSourceType::Fragment, frag);
        m_Shader = Shader::Create({vertSrc, fragSrc});

        m_VoxelBuffer = Buffer::Create(m_World->GetVoxels().size(), m_World->GetVoxels().data());

        BufferLayout verticesLayout({ShaderDataType::Float3});
        std::shared_ptr<Buffer> vertexBuffer = Buffer::Create(m_World->GetVoxels().size() * verticesLayout.GetStride());
        vertexBuffer->SetLayout(verticesLayout);

        BufferLayout indicesLayout({ShaderDataType::Int});
        std::shared_ptr<Buffer> indexBuff = Buffer::Create(m_World->GetVoxels().size() * sizeof(uint32_t), indices);
        indexBuff->SetLayout(indicesLayout);

        m_VertexArray = VertexArray::Create(vertexBuffer, indexBuff);
    }
}
