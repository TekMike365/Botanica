#include "pch.h"
#include "Application.h"

#include "Log.h"
#include "Platform.h"

#include "Layers/CameraController.h"
#include "Layers/RenderingLayer.h"
#include "Layers/SimulationLayer.h"

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
    m_Camera = Camera(fovDeg, m_Window.GetAspect(), nearPlane, farPlane);
    m_Camera.transform.SetPosition(glm::vec3(2.0f, 2.0f, 8.0f));

    m_World = World(glm::uvec3(32, 32, 32));

    m_LayerStack.PushLayer(new CameraController(m_Camera));
    m_LayerStack.PushLayer(new SimulationLayer(m_World));

    m_LayerStack.PushOverlay(new RenderingLayer(m_Camera, m_World));
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
    }
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
