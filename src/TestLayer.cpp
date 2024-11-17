#include "btpch.h"
#include "TestLayer.h"

#include "Core/Renderer/RenderCommand.h"
#include "Core/Renderer/Renderer.h"

#include "Core/Input.h"
#include "Core/Application.h"

namespace Botanica
{
    TestLayer::TestLayer()
        : Layer("TestLayer")
    {
        m_Camera = std::make_shared<Camera>(45.0f, 0.1f, 100.0f);
        m_Camera->transform.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));

        Setup();
    }

    TestLayer::~TestLayer()
    {
    }

    void TestLayer::OnAttach()
    {
        auto [x, y] = Input::GetMousePosition();
        m_LastMousePos = glm::vec2(x, y);
    }

    void TestLayer::OnUpdate(Timestep dt)
    {
        HandleInput(dt);
        Render();
    }

    void TestLayer::HandleInput(Timestep dt)
    {
        auto [x, y] = Input::GetMousePosition();
        glm::vec2 mousePos(x, y);

        glm::vec2 mouseDir = m_LastMousePos - mousePos;
        mouseDir.x /= Application::Get().GetWindow().GetWidth();
        mouseDir.y /= Application::Get().GetWindow().GetHeight();

        m_LastMousePos = mousePos;

        if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
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

            float angularSpeed = 3.141592f * 100.0f;
            glm::vec3 rotationDir = m_Camera->transform.GetRightVector() * mouseDir.y + glm::vec3(0.0f, 1.0f, 0.0f) * mouseDir.x;
            m_Camera->transform.Rotate(rotationDir * angularSpeed * dt.GetSeconds());
        }
    }

    void TestLayer::Render()
    {
        RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        RenderCommand::ClearScreen();

        Renderer::BeginScene(m_Camera);
        Renderer::Submit(m_Shader, m_VertexArray);
        Renderer::EndScene();
    }
    
    void TestLayer::Setup()
    {
        struct Vertex {
            glm::vec3 Position;
            glm::vec4 Color;
        };

        Vertex vertices[] = {
            { glm::vec3( 0.0f,  0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
            { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
            { glm::vec3( 0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) },
        };
        std::shared_ptr<Buffer> vb = Buffer::Create(3 * sizeof(Vertex), &vertices);
        vb->SetLayout(BufferLayout({
            { ShaderDataType::Float3 }, // Position
            { ShaderDataType::Float4 } // Color
        }));

        uint32_t indices[] = {0, 1, 2};
        std::shared_ptr<Buffer> ib = Buffer::Create(3 * sizeof(uint32_t), &indices);
        ib->SetLayout(BufferLayout({
            { ShaderDataType::Int },
        }));

        m_VertexArray = VertexArray::Create(vb, ib);
        m_VertexArray->IndexCount = 3;

        const char* vert = R"(
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

        const char* frag = R"(
            #version 430 core
            
            in vec4 vColor;

            out vec4 fColor;

            void main()
            {
                fColor = vColor;
            }
        )";

        m_Shader = std::shared_ptr<Shader>(Shader::Create());
        m_Shader->AddSource(ShaderSourceType::Vertex, vert);
        m_Shader->AddSource(ShaderSourceType::Fragment, frag);
    }
}
