#include "pch.h"

#include "Application.h"

#include "Log.h"

#define BIND_EVENT_CALLBACK(x) std::bind(&Application::x, this, std::placeholders::_1)

Application::Application()
{
    m_Window.SetEventCallbackFunction(BIND_EVENT_CALLBACK(OnEvent));

    WindowResizeEvent event(m_Window.GetWidth(), m_Window.GetHeight());
    m_Window.GetEventCallbackFunction()(event);
}

Application::~Application()
{
}

void Application::Run()
{
    BT_DLOG_INFO("Running application.");

    while (m_Running)
    {
        m_Window.OnUpdate();
    }
}

void Application::OnEvent(Event &e)
{
    if (e.GetCategoryFlags() & (EventCategoryButton | EventCategoryKeyboard | EventCategoryWindow))
        BT_DLOG_TRACE(e.ToString());

    EventDispatcher dispatcher(e);
    dispatcher.Dispatch<WindowCloseEvent>(BIND_EVENT_CALLBACK(OnWindowClose));
}

bool Application::OnWindowClose(WindowCloseEvent &e)
{
    BT_DLOG_WARN("Closing window.");
    m_Running = false;
    return true;
}
