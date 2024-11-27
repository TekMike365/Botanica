#pragma once

#include <memory>

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

        inline Window &GetWindow() { return *m_Window; }

        inline static Application &Get() { return *s_Instace; }
        
    private:
        void OnEvent(Event &e);

        bool OnWindowClose(WindowCloseEvent &e);

    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
        LayerStack m_LayerStack;
        double m_LastTime = 0;

        static Application *s_Instace;
    };
}
