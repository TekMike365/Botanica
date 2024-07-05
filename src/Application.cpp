#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

#include "Camera.h"

namespace Botanica
{

#define BIND_EVENT_FN(x) std::bind(&Application::x, this, std::placeholders::_1)

    Application *Application::s_Instance = nullptr;

    Application::Application()
    {
        BT_ASSERT(!s_Instance, "Application already exists!");
        s_Instance = this;

        m_Window = std::make_unique<Window>();
        m_Window->SetEventCallbackFunction(BIND_EVENT_FN(OnEvent));
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

        Renderer::VertexArray va;
        Renderer::VertexBuffer vb(vertices, sizeof(vertices));
        Renderer::VertexLayout vbl;
        vbl.PushElement(GL_FLOAT, 3, false);
        vbl.PushElement(GL_FLOAT, 3, true);
        va.AddVertexBuffer(vb, vbl);

        Renderer::IndexBuffer ia(indices, 36);

        glm::mat4 model(1.0f);

        Camera cam;
        cam.Move(glm::vec3(1.0f, 0.0f, -3.0f));
        cam.Rotate(0.0f, -10.0f);

        while (m_Running)
        {
            glClearColor(0.6f, 0.7f, 0.9f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetMat4("model", model);
            shader.SetMat4("view", cam.GetView());
            shader.SetMat4("projection", cam.GetProjection());

            va.Bind();
            glDrawElements(GL_TRIANGLES, ia.GetCount(), GL_UNSIGNED_INT, 0);

            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event &e)
    {
        // BT_TRACE("{}", e.ToString());

        EventDispatcher dispatcher(e);

        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_FN(OnWindowClose));
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        m_Running = false;
        return true;
    }

}
