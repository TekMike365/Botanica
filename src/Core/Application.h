#pragma once
#include "btpch.h"

#include "Window.h"

namespace Botanica
{
    class Application
    {
    public:
        Application();
        virtual ~Application();

        void Run();

    private:
        bool m_Running;
        std::unique_ptr<Window> m_Window;
    };
}
