#pragma once

#include "btpch.h"
#include "Window.h"

namespace Botanica {

    class Application
    {
    public:
        Application();
        ~Application();
        
        void Run();
    private:
        std::unique_ptr<Window> m_Window;
        bool m_IsRunning = true;
    };

}