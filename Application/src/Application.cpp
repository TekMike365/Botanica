#include "Application.h"

namespace Botanica
{

    Application::Application()
    {
        m_Window = std::unique_ptr<Window>(Window::CreateWindow());
    }

    Application::~Application() {}

    void Application::Run()
    {
        BT_INFO("Hello, Botanica!");

        while (m_IsRunning)
        {
            m_Window->OnUpdate();
        }
    }

}
