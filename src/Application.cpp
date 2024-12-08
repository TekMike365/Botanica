#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Platform.h"

#include "Components/Camera.h"
#include "Layers/CameraController.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&Application::x, this, std::placeholders::_1)

Application *Application::s_Instance = nullptr;

Application::Application()
{
    BT_ASSERT(!s_Instance, "application already exists.")
    s_Instance = this;

    m_Window.SetEventCallbackFunction(BIND_EVENT_CALLBACK(OnEvent));

    WindowResizeEvent event(m_Window.GetWidth(), m_Window.GetHeight());
    m_Window.GetEventCallbackFunction()(event);

    float fovDeg = 60.0f;
    float nearPlane = 0.1f;
    float farPlane = 100.0f;
    Camera cam(fovDeg, m_Window.GetAspect(), nearPlane, farPlane);
    cam.transform.SetPosition(glm::vec3(0.0f, 0.0f, 3.0f));
    m_CameraController = new CameraController(cam);
    m_LayerStack.PushLayer(m_CameraController);

    Setup();
}

Application::~Application()
{
}

void Application::Run()
{
    BT_DLOG_INFO("Running application.");

    while (m_Running)
    {
        double nextTime = Platform::GetTime();
        Timestep dt = nextTime - m_LastTime;
        m_LastTime = nextTime;

        m_Window.OnUpdate();

        for (Layer *layer : m_LayerStack)
            layer->OnUpdate(dt);

        OnUpdate(dt);
    }
}

void Application::Setup()
{
    const char *vert = R"(
        #version 430 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aCol;

        uniform mat4 uVP;

        out vec4 vCol;

        void main()
        {
            gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
            vCol = aCol;
        }
    )";

    const char *frag = R"(
        #version 430 core

        in vec4 vCol;

        out vec4 fColor;

        void main()
        {
            fColor = vCol;
        }
    )";

    struct Vertex
    {
        glm::vec3 Pos;
        glm::vec4 Col;
    };

    const Vertex vertices[] = {
        { glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f) },
        { glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f) },
        { glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f) }
    };

    const uint32_t indices[] = { 0, 1, 2 };

    Renderer::BufferLayout vbl({
        { Renderer::ShaderDataType::Float3 },
        { Renderer::ShaderDataType::Float4 }
    });

    Renderer::BufferLayout ibl({
        { Renderer::ShaderDataType::UInt }
    });

    std::shared_ptr<Renderer::Buffer> vb = std::make_shared<Renderer::Buffer>(sizeof(vertices), (const void*)vertices);
    vb->SetLayout(vbl);
    std::shared_ptr<Renderer::Buffer> ib = std::make_shared<Renderer::Buffer>(sizeof(indices), (const void*)indices);
    ib->SetLayout(ibl);
    m_VertexArray = std::make_shared<Renderer::VertexArray>(vb, ib);

    Renderer::ShaderSource vertSrc(Renderer::ShaderSourceType::Vertex, vert);
    Renderer::ShaderSource fragSrc(Renderer::ShaderSourceType::Fragment, frag);
    m_Shader = std::shared_ptr<Renderer::Shader>(new Renderer::Shader({ &vertSrc, &fragSrc }));
}

void Application::OnUpdate(Timestep dt)
{
    Renderer::SetClearColor(glm::vec4(0.5f, 0.7f, 0.8f, 1.0f));
    Renderer::ClearScreen();

    m_VertexArray->Bind();
    m_Shader->Bind();
    m_Shader->UploadUniform(Renderer::UniformType::Mat4, "uVP", (const void *)&m_CameraController->GetCamera().GetVPMat());

    Renderer::DrawIndexed(3);
}

void Application::OnEvent(Event &e)
{
    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_CALLBACK(OnWindowClose));

    for (auto it = m_LayerStack.end(); it != m_LayerStack.begin();)
        (*--it)->OnEvent(e);
}

bool Application::OnWindowClose(WindowCloseEvent &e)
{
    BT_DLOG_WARN("Closing window.");
    m_Running = false;
    return true;
}
