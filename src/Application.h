#pragma once

#include "Window.h"
#include "LayerStack.h"
#include "Event/WindowEvent.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

    inline const Window &GetWindow() const { return m_Window; }

    static const Application &Get() { return *s_Instance; }

private:
    void OnEvent(Event &e);
    bool OnWindowClose(WindowCloseEvent &e);

private:
    bool m_Running = true;
    Window m_Window;
    LayerStack m_LayerStack;
    double m_LastTime = 0;
    
    static Application *s_Instance;
};