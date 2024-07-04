#pragma once

#include "btpch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "LayerStack.h"

namespace Botanica {

    class Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        void OnEvent(Event& e);

        void PushLayer(Layer* layer);

        inline Application& Get() const { return *s_Instance; }

    private:
        bool OnWindowClose(WindowCloseEvent& e);

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        LayerStack m_LayerStack;
        static Application* s_Instance;
    };

}