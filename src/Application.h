#pragma once

#include <memory>

#include "Timestep.h"
#include "Window.h"
#include "Event/WindowEvent.h"

#include "LayerStack.h"
#include "Layers/CameraController.h"

#include "Renderer/VertexArray.h"
#include "Renderer/Shader.h"

class Application
{
public:
    Application();
    ~Application();

    void Run();

    inline const Window &GetWindow() const { return m_Window; }

    static const Application &Get() { return *s_Instance; }

private:
    void Setup();

    void OnUpdate(Timestep dt);
    void OnEvent(Event &e);
    bool OnWindowClose(WindowCloseEvent &e);

private:
    bool m_Running = true;
    Window m_Window;
    LayerStack m_LayerStack;
    double m_LastTime = 0;

    CameraController *m_CameraController;

    std::shared_ptr<Renderer::VertexArray> m_VertexArray;
    std::shared_ptr<Renderer::Shader> m_Shader;
    
    static Application *s_Instance;
};