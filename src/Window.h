#pragma once

#include "btpch.h"
#include "Events/Event.h"

#include <GLFW/glfw3.h>

namespace Botanica
{

    struct WindowParams
    {
        std::string Title;
        unsigned int Width, Height;

        WindowParams(const std::string& title = "Botanica", unsigned int widht = 1080, unsigned int height = 720)
            :Title(title), Width(widht), Height(height) {}
    };


    class Window
    {
    public:
        using EventCallbackFn = std::function<void(Event&)>;

        Window(const WindowParams& params = WindowParams());
        virtual ~Window();

        void OnUpdate();

        inline unsigned int GetWidth() const { return m_Data.Width; }
        inline unsigned int GetHeight() const { return m_Data.Height; }
        inline float GetAspect() const { return (float)m_Data.Width / (float)m_Data.Height; }

        inline void SetEventCallbackFunction(const EventCallbackFn& fn) { m_Data.EventCallback = fn; }
        void SetVSync(bool enabled);
        inline bool IsVSync() const { return m_Data.VSync; }

        inline void HideCursor() { glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_HIDDEN | GLFW_CURSOR_DISABLED); }
        inline void ShowCursor() { glfwSetInputMode(m_Window, GLFW_CURSOR, GLFW_CURSOR_NORMAL); }
    private:
        GLFWwindow* m_Window;

        struct WindowData
        {
            std::string Title;
            unsigned int Width, Height;
            bool VSync;

            EventCallbackFn EventCallback;
        };

        WindowData m_Data;
    };

}
