#include "btpch.h"

#include "Log.h"
#include "Application.h"

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

        while (m_Running)
        {
            m_Window->OnUpdate();
        }
    }

    void Application::OnEvent(Event &e)
    {
        BT_CORE_TRACE(e.ToString());

        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_CALLBACK(OnWindowClose));
    }

    bool Application::OnWindowClose(WindowCloseEvent &e)
    {
        BT_CORE_WARN("Closing window ({})", e.ToString());
        m_Running = false;
        return true;
    }

}