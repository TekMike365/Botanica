#pragma once

#include "Event.h"

namespace Botanica
{

    class WindowCloseEvent : public Event
    {
    public:
        WindowCloseEvent() {}

        EVENT_CLASS_TYPE(WindowClose)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class WindowResizeEvent : public Event
    {
    public:
        WindowResizeEvent(unsigned int width, unsigned int height)
            :m_Width(width), m_Height(height) {}

        inline unsigned int GetWidth() const { return m_Width; }
        inline unsigned int GetHeight() const { return m_Height; }

        EVENT_CLASS_TYPE(WindowResize)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_Width << ", " << m_Height;
            return ss.str();
        }
    private:
        unsigned int m_Width;
        unsigned int m_Height;
    };

    class WindowFocusEvent : public Event
    {
    public:
        WindowFocusEvent() {}

        EVENT_CLASS_TYPE(WindowFocus)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

    class WindowLostFocusEvent : public Event
    {
    public:
        WindowLostFocusEvent() {}

        EVENT_CLASS_TYPE(WindowLostFocus)
        EVENT_CLASS_CATEGORY(EventCategoryApplication)
    };

}
