#include "btpch.h"
#include "RenderLayer.h"

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

const float VERTICES[] = {
    // 0
    0.0f, 0.5f, 0.0f,
    1.0f, 0.0f, 0.0f, 1.0f,
    // 1
    -0.5f, -0.5f, 0.0f,
    0.0f, 1.0f, 0.0f, 1.0f,
    // 2
    0.5f, -0.5f, 0.0f,
    0.0f, 0.0f, 1.0f, 1.0f};

const uint32_t INDICES[] = {0, 1, 2};

const char *VERTEX_SHADER_SOURCE = R"(
    #version 330 core
    layout (location = 0) in vec3 a_Position;
    layout (location = 1) in vec4 a_Color;

    out vec4 v_Color;

    void main()
    {
        gl_Position = vec4(a_Position, 1.0);
        v_Color = a_Color;
    }
)";

const char *FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core
    out vec4 color;

    in vec4 v_Color;

    void main()
    {
        color = v_Color;
    }
)";

namespace Botanica
{
    RenderLayer::RenderLayer()
        : Layer("RenderLayer")
    {
        BufferLayout layout;
        layout.PushElement({ShaderDataType::Float3, "a_Position", false});
        layout.PushElement({ShaderDataType::Float4, "a_Color", false});

        VertexBuffer *vertexBuffer = VertexBuffer::Create(sizeof(VERTICES), VERTICES);
        vertexBuffer->SetLayout(layout);
        IndexBuffer *indexBuffer = IndexBuffer::Create(sizeof(INDICES) / sizeof(uint32_t), INDICES);

        m_VertexArray.reset(VertexArray::Create(vertexBuffer, indexBuffer));

        m_Shader.reset(Shader::Create());
        m_Shader->AddSource(ShaderSourceType::Vertex, VERTEX_SHADER_SOURCE);
        m_Shader->AddSource(ShaderSourceType::Fragment, FRAGMENT_SHADER_SOURCE);
    }

    RenderLayer::~RenderLayer()
    {
    }

    void RenderLayer::OnUpdate()
    {
        RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        RenderCommand::ClearScreen();

        Renderer::BeginScene();

        Renderer::Submit(m_Shader, m_VertexArray);

        Renderer::EndScene();
    }
}