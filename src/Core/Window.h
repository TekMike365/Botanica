#pragma once
#include "btpch.h"

#include "Event/Event.h"

namespace Botanica
{
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

        virtual ~Window() {}

        virtual void OnUpdate() = 0;

        virtual unsigned int GetWidth() const = 0;
        virtual unsigned int GetHeight() const = 0;

        virtual void SetEventCallbackFunction(const EventCallbackFn &fn) = 0;
        virtual const EventCallbackFn &GetEventCallbackFunction() const = 0;

        virtual void SetVSync(bool enabled) = 0;
        virtual bool IsVSync() const = 0;

        static Window *Create(const WindowProps &props = WindowProps());
    };
}