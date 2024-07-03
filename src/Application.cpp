#include <GLFW/glfw3.h>
#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

#include "Application.h"

#include "Renderer/VertexArray.h"
#include "Renderer/VertexBuffer.h"
#include "Renderer/IndexBuffer.h"
#include "Renderer/Shader.h"

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
            { glm::vec3(-0.5f, -0.5f, -0.5f), glm::vec3(1.0f, 0.0f, 0.0f) },
            { glm::vec3( 0.5f, -0.5f,  0.5f), glm::vec3(0.0f, 1.0f, 0.0f) },
            { glm::vec3(-0.5f,  0.5f,  0.5f), glm::vec3(0.0f, 0.0f, 1.0f) }
        };

        uint32_t indices[] = {
            0, 1, 2
        };

        Shader shader("shaders/Test.vert", "shaders/Test.frag");

        VertexArray va;
        VertexBuffer vb(vertices, sizeof(vertices));
        VertexLayout vbl;
        vbl.PushElement({ GL_FLOAT, 3, GL_FALSE, sizeof(glm::vec3) });
        vbl.PushElement({ GL_FLOAT, 3, GL_TRUE, sizeof(glm::vec3) });
        va.AddVertexBuffer(vb, vbl);

        IndexBuffer ia(indices, 3);

        glm::mat4 model = glm::mat4(1.0f);
        glm::mat4 view = glm::mat4(1.0f);
        view = glm::translate(view, glm::vec3(0.0f, 0.0f, -3.0f));

        glm::mat4 projection;
        float fov = glm::radians(45.0f);
        float aspect = m_Window->GetAspect();
        float nearPlane = 0.4f;
        float farPlane = 100.0f;
        projection = glm::perspective(fov, aspect, nearPlane, farPlane);

        while (m_Running)
        {
            glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            shader.Bind();
            shader.SetMat4("model", model);
            shader.SetMat4("view", view);
            shader.SetMat4("projection", projection);

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
