#pragma once
#include "btpch.h"

#include "Window.h"
#include "LayerStack.h"

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
        void PushLayer(Layer *layer);
        void PushOverlay(Layer *overlay);

    private:
        void OnEvent(Event &e);

        bool OnWindowClose(WindowCloseEvent &e);

    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
    };
}
