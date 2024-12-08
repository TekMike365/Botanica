#pragma once

#include "Window.h"
#include "Event/WindowEvent.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

private:
    void OnEvent(Event &e);
    bool OnWindowClose(WindowCloseEvent &e);

private:
    bool m_Running = true;
    Window m_Window;
};