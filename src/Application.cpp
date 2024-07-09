#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

#include "Renderer/Mesh.h"

#include "Camera.h"

namespace Botanica
{

    Application *Application::s_Instance = nullptr;

    Application::Application()
        :m_Player(4.0f/3.0f)
    {
        BT_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::make_unique<Window>();
        m_Window->SetEventCallbackFunction(BIND_EVENT_FN(Application::OnEvent));


        m_Player = Player(m_Window->GetAspect());
    }

    Application::~Application() {}

    void Application::Run()
    {
        BT_INFO("Running application.");

        struct Vertex
        {
            glm::vec3 Position;
            glm::vec3 Color;
        };

        Vertex vertices[] = {
            // bottom
            { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(0.0f, 0.0f, 0.0f) },
            { glm::vec3( 0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
            { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(1.0f, 0.0f, 1.0f) },
            { glm::vec3(-0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f) },

            // top
            { glm::vec3(-0.5f,  0.5f, -0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },
            { glm::vec3( 0.5f,  0.5f, -0.5f), glm::vec3(1.0f, 1.0f, 0.0f) },
            { glm::vec3( 0.5f,  0.5f,  0.5f), glm::vec3(1.0f, 1.0f, 1.0f) },
            { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 1.0f) }
        };

        uint32_t indices[] = {
            // top
            4, 7, 6,
            6, 5, 4,
            // bottom
            3, 0, 1,
            1, 2, 3,
            // left
            3, 7, 4,
            4, 0, 3,
            // right
            1, 5, 6,
            6, 2, 1,
            // front
            0, 4, 5,
            5, 1, 0,
            // back
            2, 6, 7,
            7, 3, 2
        };

        Renderer::Shader shader("shaders/Test.vert", "shaders/Test.frag");

        Renderer::VertexLayout vertexLayout;
        vertexLayout.PushElement(GL_FLOAT, 3, false);
        vertexLayout.PushElement(GL_FLOAT, 3, true);

        Renderer::Mesh mesh(vertices, 8 * sizeof(Vertex), vertexLayout, indices, 36);

        glm::mat4 model(1.0f);
        model = glm::translate(model, glm::vec3(0.0f, 0.0f, -5.0f));

        const Camera& cam = m_Player.GetCamera();

        glEnable(GL_DEPTH_TEST);

        double oldTime = glfwGetTime();
        float deltaTime = 0;
        while (m_Running)
        {
            glClearColor(0.6f, 0.7f, 0.9f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

            shader.SetMat4("model", model);
            shader.SetMat4("view", cam.GetView());
            shader.SetMat4("projection", cam.GetProjection());
            mesh.Render(shader);

            m_Window->OnUpdate();
            if (m_EnablePlayer)
                m_Player.OnUpdate(deltaTime);

            double newTime = glfwGetTime();
            deltaTime = (float)(newTime - oldTime);
            oldTime = newTime;
        }
    }

    void Application::OnEvent(Event &e)
    {
        // BT_TRACE("{}", e.ToString());

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(Application::OnWindowClose));
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Application::OnKeyPressed));
        dispatcher.Dispatch<MouseButtonReleasedEvent>(BIND_EVENT_FN(Application::OnMouseButtonReleased));

        if (m_EnablePlayer)
            m_Player.OnEvent(e);
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

    bool Application::OnKeyPressed(KeyPressedEvent &e)
    {
        if (e.GetKey() == GLFW_KEY_ESCAPE && m_EnablePlayer)
        {
            m_EnablePlayer = false;
            m_Window->ShowCursor();
            return true;
        }

        return false;
    }

    bool Application::OnMouseButtonReleased(MouseButtonReleasedEvent &e)
    {
        if (e.GetButton() == GLFW_MOUSE_BUTTON_1 && !m_EnablePlayer)
        {
            m_EnablePlayer = true;
            m_Window->HideCursor();
            
            double mouseX, mouseY;
            m_Window->GetMousePos(&mouseX, &mouseY);
            m_Player.OnEnable((float)mouseX / (float)m_Window->GetWidth(), (float)mouseY / (float)m_Window->GetHeight());
            return true;
        }

        return false;
    }

}
