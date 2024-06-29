#pragma once

#include "btpch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"

namespace Botanica {

    class Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        void OnEvent(Event& e);
    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
    };

}