#pragma once

#include <sstream>

#include "Event.h"

namespace Botanica
{
    class WindowCloseEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            : m_Width(width), m_Height(height) {}

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)

        virtual inline std::string ToString() const
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_Width << ", " << m_Height;
            return ss.str();
        }
    private:
        unsigned int m_Width, m_Height;
    };

    class WindowFocusEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };

    class WindowLostFocusEvent : public Event
    {
    public:
        EVENT_CLASS_TYPE(WindowLostFocus)
        EVENT_CLASS_CATEGORY(EventCategoryWindow)
    };
}