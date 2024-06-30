#pragma once

#include "Event.h"

namespace Botanica
{

    class MouseMovedEvent : public Event
    {
    public:
        MouseMovedEvent(float xpos, float ypos)
            :m_XPos(xpos), m_YPos(ypos) {}

        inline float GetXPos() const { return m_XPos; }
        inline float GetYPos() const { return m_YPos; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
        EVENT_CLASS_TYPE(MouseMoved)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_XPos << ", " << m_YPos;
            return ss.str();
        }
    private:
        float m_XPos, m_YPos;
    };

    class MouseScrolledEvent : public Event
    {
    public:
        MouseScrolledEvent(float xoffset, float yoffset)
            :m_XOffset(xoffset), m_YOffset(yoffset) {}

        inline float GetXOffset() const { return m_XOffset; }
        inline float GetYOffset() const { return m_YOffset; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse)
        EVENT_CLASS_TYPE(MouseScrolled)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_XOffset << ", " << m_YOffset;
            return ss.str();
        }
    private:
        float m_XOffset, m_YOffset;
    };

    class MouseButtonEvent : public Event
    {
    public:
        MouseButtonEvent(int button)
            :m_Button(button) {}

        inline int GetButton() const { return m_Button; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryMouse | EventCategoryButton)
    protected:
        int m_Button;
    };

    class MouseButtonPressedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonPressedEvent(int button)
            :MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonPressed)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_Button;
            return ss.str();
        }
    };

    class MouseButtonReleasedEvent : public MouseButtonEvent
    {
    public:
        MouseButtonReleasedEvent(int button)
            :MouseButtonEvent(button) {}

        EVENT_CLASS_TYPE(MouseButtonReleased)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_Button;
            return ss.str();
        }
    };

}
