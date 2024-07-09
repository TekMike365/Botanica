#pragma once

#include "btpch.h"
#include "Window.h"

#include "Events/ApplicationEvent.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"

#include "Player.h"

namespace Botanica {

    class Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
        void OnEvent(Event& e);

        inline Application& Get() const { return *s_Instance; }
    private:
        bool OnWindowClose(WindowCloseEvent& e);
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnMouseButtonReleased(MouseButtonReleasedEvent& e);
    private:
        Player m_Player;
        bool m_EnablePlayer = false;

        std::unique_ptr<Window> m_Window;
        bool m_Running = true;
        static Application* s_Instance;
    };

}