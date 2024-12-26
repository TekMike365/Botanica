#pragma once

#include <memory>

#include "Timestep.h"
#include "Window.h"
#include "Event/WindowEvent.h"

#include "LayerStack.h"

#include "Components/Camera.h"
#include "Components/World.h"

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

    Camera m_Camera;
    World m_World;
 
    static Application *s_Instance;
};