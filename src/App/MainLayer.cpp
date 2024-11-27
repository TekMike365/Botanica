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
        : Layer("TestLayer"), m_CameraController(new CameraController())
    {
        m_ObjStack.PushLayer(m_CameraController);

        Setup();
    }

    MainLayer::~MainLayer()
    {
    }

    void MainLayer::OnAttach()
    {
        Renderer::RenderPipeline pipeline({ std::bind(&MainLayer::TestRenderPass, this) });
        Renderer::SetRenderPipeline(pipeline);

        for (auto &e : m_ObjStack)
            e->OnAttach();
    }

    void MainLayer::OnUpdate(Timestep dt)
    {
        for (auto &e : m_ObjStack)
            e->OnUpdate(dt);
    }

    void MainLayer::Setup()
    {
        struct Vertex
        {
            glm::vec3 Position;
            glm::vec4 Color;
        };

        Vertex vertices[] = {
            {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
            {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
            {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)},
        };
        std::shared_ptr<Renderer::Buffer> vb = Renderer::Buffer::Create(3 * sizeof(Vertex), &vertices);
        vb->SetLayout(Renderer::BufferLayout({
            {Renderer::ShaderDataType::Float3}, // Position
            {Renderer::ShaderDataType::Float4}  // Color
        }));

        uint32_t indices[] = {0, 1, 2};
        std::shared_ptr<Renderer::Buffer> ib = Renderer::Buffer::Create(3 * sizeof(uint32_t), &indices);
        ib->SetLayout(Renderer::BufferLayout({
            {Renderer::ShaderDataType::Int},
        }));

        m_VertexArray = Renderer::VertexArray::Create(vb, ib);
        m_VertexArray->IndexCount = 3;

        const char *vert = R"(
            #version 430 core
            layout (location = 0) in vec3 aPos;
            layout (location = 1) in vec4 aColor;

            out vec4 vColor;

            uniform mat4 uVP;

            void main()
            {
                gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
                vColor = aColor;
            }
        )";

        const char *frag = R"(
            #version 430 core
            
            in vec4 vColor;

            out vec4 fColor;

            void main()
            {
                fColor = vColor;
            }
        )";

        std::shared_ptr<Renderer::ShaderSource> vs = Renderer::ShaderSource::Create(Renderer::ShaderSourceType::Vertex, vert);
        std::shared_ptr<Renderer::ShaderSource> fs = Renderer::ShaderSource::Create(Renderer::ShaderSourceType::Fragment, frag);
        m_Shader = Renderer::Shader::Create({vs, fs});
    }

    void MainLayer::TestRenderPass()
    {
        using namespace Renderer;

        RenderCommand::SetRenderState({.ShaderPtr = m_Shader,
                                       .VertexArrayPtr = m_VertexArray,
                                       .ClearColor = glm::vec4(0.2f, 0.3f, 0.3f, 1.0f)});
        RenderCommand::ClearScreen();

        std::vector<Uniform> uniforms;
        uniforms.emplace_back(UniformType::Mat4, "uVP", std::make_shared<glm::mat4>(m_CameraController->GetCamera().GetVPMat()));
        RenderCommand::SetShaderUniforms(uniforms);

        RenderCommand::DrawIndexed(m_VertexArray->IndexCount, 0);
    }
}
