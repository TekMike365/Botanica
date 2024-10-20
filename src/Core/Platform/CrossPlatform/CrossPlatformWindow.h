#pragma once

#include <string>

#include <GLFW/glfw3.h>

#include "Core/Window.h"
#include "Core/Renderer/GraphicsContext.h"


namespace Botanica
{
    class LinuxWindow : public Window
    {
    public:
        LinuxWindow(const WindowProps &props);
        virtual ~LinuxWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_WindowData.Width; }
        inline unsigned int GetHeight() const override { return m_WindowData.Height; }

        virtual inline void SetEventCallbackFunction(const EventCallbackFn &fn) override { m_WindowData.EventCallback = fn; }
        virtual inline const EventCallbackFn &GetEventCallbackFunction() const override { return m_WindowData.EventCallback; }

        void SetVSync(bool enabled) override;
        inline bool IsVSync() const override { return m_WindowData.VSync; }

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
        GraphicsContext *m_Context;
    };
}
