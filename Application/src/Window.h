#pragma once

#include "btpch.h"

namespace Botanica
{

    struct WindowParams
    {
        std::string Title;
        unsigned int Width, Height;

        WindowParams(const std::string& title = "Botanica", unsigned int widht = 1280, unsigned int height = 720)
            :Title(title), Width(widht), Height(height) {}
    };

    class Window
    {
    public:
        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window* CreateWindow(const WindowParams& params=WindowParams());
    protected:
        WindowParams m_Params;
    };

}
