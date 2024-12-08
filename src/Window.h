#pragma once

#include <string>
#include <functional>
#include <GLFW/glfw3.h>

#include "Event/Event.h"

struct WindowProps
{
    std::string Title;
    unsigned int Width, Height;

    WindowProps(const std::string &title = "Botanica", unsigned int width = 1280, unsigned int height = 720)
        : Title(title), Width(width), Height(height) {}
};

class Window
{
public:
    using EventCallbackFn = std::function<void(Event &)>;

    Window(const WindowProps &props = WindowProps());
    ~Window();

    void OnUpdate();

    inline unsigned int GetWidth() const { return m_WindowData.Width; }
    inline unsigned int GetHeight() const { return m_WindowData.Height; }
    inline float GetAspect() const { return m_WindowData.Width / m_WindowData.Height; }

    inline void SetEventCallbackFunction(const EventCallbackFn &fn) { m_WindowData.EventCallback = fn; }
    inline const EventCallbackFn &GetEventCallbackFunction() const { return m_WindowData.EventCallback; }

    void SetVSync(bool enabled);
    inline bool IsVSync() const { return m_WindowData.VSync; }

private:
    struct WindowData
    {
        std::string Title;
        unsigned int Width, Height;
        bool VSync;

        EventCallbackFn EventCallback;
    };

private:
    void Init();

private:
    WindowData m_WindowData;
    GLFWwindow *m_Window;
};