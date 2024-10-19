#pragma once
#include "btpch.h"

#include "Window.h"

#include "Event/Event.h"
#include "Event/WindowEvent.h"

namespace Botanica
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        void OnEvent(Event &e);

        bool OnWindowClose(WindowCloseEvent &e);

    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
    };
}
