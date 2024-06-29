#pragma once

#include "btpch.h"
#include "Window.h"
#include "GLFW/glfw3.h"

namespace Botanica
{

    class GLFWWindow : public Window
    {
    public:
        GLFWWindow(const WindowParams& params);
        virtual ~GLFWWindow();

        void OnUpdate() override;

        inline unsigned int GetWidth() const override { return m_Data.Width; }
        inline unsigned int GetHeight() const override { return m_Data.Height; }

        inline void SetEventCallbackFunction(const EventCallbackFn& fn) override { m_Data.EventCallback = fn; }
        void SetVSync(bool enabled) override;
        inline bool IsVSync() const override { return m_Data.VSync; }
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
