#include "btpch.h"
#include "Application.h"

#include "Platform.h"

#include "TestLayer.h"
#include "RenderLayer.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&Application::x, this, std::placeholders::_1)

namespace Botanica
{
    Application *Application::s_Instace = nullptr;

    Application::Application()
    {
        BT_CORE_ASSERT(!s_Instace, "Can have only one Application!")
        s_Instace = this;

        m_Window = std::unique_ptr<Window>(Window::Create());
        m_Window->SetEventCallbackFunction(BIND_EVENT_CALLBACK(OnEvent));

        m_LayerStack.PushLayer(new RenderLayer);

        WindowResizeEvent event(m_Window->GetWidth(), m_Window->GetHeight());
        m_Window->GetEventCallbackFunction()(event);

        m_LayerStack.PushLayer(new TestLayer);
    }

    Application::~Application()
    {
    }

    void Application::Run()
    {
        BT_CORE_INFO("Running application.");

        while (m_Running)
        {
            double nextTime = Platform::GetTime();
            Timestep dt = nextTime - m_LastTime;
            m_LastTime = nextTime;
            
            m_Window->OnUpdate();

            for (Layer *layer : m_LayerStack)
                layer->OnUpdate(dt);
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
