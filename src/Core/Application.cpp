#include "btpch.h"

#include <glad/glad.h>

#include "Log.h"
#include "Application.h"

#include "Platform/OpenGL/Shader.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Botanica
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFunction(BIND_EVENT_CALLBACK(OnEvent));

        WindowResizeEvent event(m_Window->GetWidth(), m_Window->GetHeight());
        m_Window->GetEventCallbackFunction()(event);
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        BT_CORE_INFO("Running application.");

        float vertices[] = {
             0.0f,  0.5f, 0.0f,
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
        };

        unsigned int indices[] = {0, 1, 2};

        unsigned int vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        unsigned int vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);
        
        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, nullptr);
        glEnableVertexAttribArray(0);

        unsigned int ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        const char *vertexShaderSource = R"(
            #version 330 core
            layout (location = 0) in vec3 pos;

            void main()
            {
                gl_Position = vec4(pos, 1.0);
            }
        )";

        const char *fragmentShaderSource = R"(
            #version 330 core
            out vec4 FragColor;

            void main()
            {
                FragColor = vec4(1.0f, 0.5f, 0.2f, 1.0f);
            }
        )";

        OpenGL::Shader shader;
        shader.PushSource(OpenGL::ShaderSourceType::Vertex, vertexShaderSource);
        shader.PushSource(OpenGL::ShaderSourceType::Fragment, fragmentShaderSource);

        while (m_Running)
        {
            glClearColor(0.2f, 0.3f, 0.3f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
            
            glBindVertexArray(vao);
            shader.Bind();
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_INT, 0);
            glBindVertexArray(0);

            m_Window->OnUpdate();

            for (Layer *layer : m_LayerStack)
                layer->OnUpdate();
        }
    }

    void Application::PushLayer(Layer *layer)
    {
        m_LayerStack.PushLayer(layer);
    }

    void Application::PushOverlay(Layer *overlay)
    {
        m_LayerStack.PushOverlay(overlay);
    }

    void Application::OnEvent(Event &e)
    {
        // BT_CORE_TRACE(e.ToString());

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_CALLBACK(OnWindowClose));

        for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
            (*--it)->OnEvent(e);
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        BT_CORE_WARN("Closing window ({})", e.ToString());
        m_Running = false;
        return true;
    }

}