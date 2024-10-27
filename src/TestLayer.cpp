#include "btpch.h"
#include "TestLayer.h"

#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"

#include "Core/Input.h"
#include "Core/Application.h"

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

    uniform mat4 u_VP;

    out vec4 v_Color;

    vec4 Run()
    {
        v_Color = a_Color;
        return u_VP * vec4(a_Position, 1.0);
    }
)";

const char *VERTEX_SHADER_CORE_SOURCE = R"(
    #version 330 core

    vec4 Run();

    void main()
    {
        gl_Position = Run();
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
    TestLayer::TestLayer()
        : Layer("TestLayer")
    {
        BufferLayout layout;
        layout.PushElement({ShaderDataType::Float3, "a_Position", false});
        layout.PushElement({ShaderDataType::Float4, "a_Color", false});

        VertexBuffer *vertexBuffer = VertexBuffer::Create(sizeof(VERTICES), VERTICES);
        vertexBuffer->SetLayout(layout);
        IndexBuffer *indexBuffer = IndexBuffer::Create(sizeof(INDICES) / sizeof(uint32_t), INDICES);

        m_VertexArray.reset(VertexArray::Create(vertexBuffer, indexBuffer));

        m_Shader.reset(Shader::Create());
        m_Shader->AddSource(ShaderSourceType::Vertex, VERTEX_SHADER_CORE_SOURCE);
        m_Shader->AddSource(ShaderSourceType::Vertex, VERTEX_SHADER_SOURCE);
        m_Shader->AddSource(ShaderSourceType::Fragment, FRAGMENT_SHADER_SOURCE);

        m_Camera = std::make_shared<Camera>(45.0f, 0.1f, 100.0f);
        m_Camera->transform.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

        auto [x, y] = Input::GetMousePosition();
        m_LastMousePos = glm::vec2(x, y);
    }

    TestLayer::~TestLayer()
    {
    }

    void TestLayer::OnUpdate(Timestep dt)
    {
        float speed = 4.0f;
        if (Input::IsKeyPressed(GLFW_KEY_W))
            m_Camera->transform.Translate(m_Camera->transform.GetForwardVector() * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_S))
            m_Camera->transform.Translate(m_Camera->transform.GetForwardVector() * -speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_D))
            m_Camera->transform.Translate(m_Camera->transform.GetRightVector() * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_A))
            m_Camera->transform.Translate(m_Camera->transform.GetRightVector() * -speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            m_Camera->transform.Translate(m_Camera->transform.GetUpVector() * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
            m_Camera->transform.Translate(m_Camera->transform.GetUpVector() * -speed * dt.GetSeconds());

        auto [x, y] = Input::GetMousePosition();
        glm::vec2 mousePos(x, y);

        glm::vec2 mouseDir = m_LastMousePos - mousePos;
        mouseDir.x /= Application::Get().GetWindow().GetWidth();
        mouseDir.y /= Application::Get().GetWindow().GetHeight();

        m_LastMousePos = mousePos;

        float angularSpeed = 3.141592f * 100.0f;
        glm::vec3 rotationDir = m_Camera->transform.GetRightVector() * mouseDir.y + m_Camera->transform.GetUpVector() * mouseDir.x;
        m_Camera->transform.Rotate(rotationDir * angularSpeed * dt.GetSeconds());

        RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        RenderCommand::ClearScreen();

        Renderer::BeginScene(m_Camera);

        Renderer::Submit(m_Shader, m_VertexArray);

        Renderer::EndScene();
    }
}