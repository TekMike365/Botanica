#include "btpch.h"

#include "Log.h"
#include "Application.h"

namespace Botanica
{
    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::Create());
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
}