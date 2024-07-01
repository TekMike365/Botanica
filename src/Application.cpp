#include "Application.h"

#include <GLFW/glfw3.h>
#include <glad/glad.h>

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

        const char *vsSrc =
            "#version 330 core\n"
            "layout (location = 0) in vec3 inPosition;\n"
            "void main()\n"
            "{\n"
            "    gl_Position = vec4(inPosition, 1.0);\n"
            "}\n";

        const char *fsSrc =
            "#version 330 core\n"
            "out vec4 fragColor;\n"
            "void main()\n"
            "{\n"
            "    fragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
            "}\n";

        // object id variables

        float vertices[] = {
            -0.5f, -0.5f, 0.0f,
             0.5f, -0.5f, 0.0f,
             0.0f,  0.5f, 0.0f
        };

        uint8_t indices[] = {
            0, 1, 2
        };

        int status = gladLoadGLLoader((GLADloadproc)glfwGetProcAddress);
        BT_ASSERT(status, "Glad it fucked up!");

        // create program object
        uint32_t pId = glCreateProgram();

        // create shaders
        uint32_t vertexShaderId = glCreateShader(GL_VERTEX_SHADER);
        glShaderSource(vertexShaderId, 1, &vsSrc, NULL);
        glCompileShader(vertexShaderId);

        uint32_t fragmentShaderId = glCreateShader(GL_FRAGMENT_SHADER);
        glShaderSource(fragmentShaderId, 1, &fsSrc, NULL);
        glCompileShader(fragmentShaderId);

        // attachment of shaders to program object
        glAttachShader(pId, vertexShaderId);
        glAttachShader(pId, fragmentShaderId);

        glLinkProgram(pId);

        // buffer
        uint32_t vao;
        glGenVertexArrays(1, &vao);
        glBindVertexArray(vao);

        uint32_t ebo;
        glGenBuffers(1, &ebo);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW);

        uint32_t vbo;
        glGenBuffers(1, &vbo);
        glBindBuffer(GL_ARRAY_BUFFER, vbo);
        glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

        glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(float), (void *)0);
        glEnableVertexAttribArray(0);

        while (m_Running)
        {
            glClearColor(0.0f, 0.2f, 0.2f, 1.0f);
            glClear(GL_COLOR_BUFFER_BIT);

            glUseProgram(pId);
            glBindVertexArray(vao);
            glDrawElements(GL_TRIANGLES, 3, GL_UNSIGNED_BYTE, 0);

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
