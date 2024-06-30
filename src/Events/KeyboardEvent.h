#pragma once

#include "Event.h"

namespace Botanica
{

    class  KeyEvent : public Event
    {
    public:
        KeyEvent(int key)
            :m_Key(key) {}

        inline int GetKey() const { return m_Key; }

        EVENT_CLASS_CATEGORY(EventCategoryInput | EventCategoryButton | EventCategoryKeyboard)
    protected:
        int m_Key;
    };

    class KeyPressedEvent : public KeyEvent
    {
    public:
        KeyPressedEvent(int key, int repeatCount)
            :KeyEvent(key), m_RepeatCount(repeatCount) {}

        inline int GetRepeatCount() const { return m_RepeatCount; }

        EVENT_CLASS_TYPE(KeyPressed)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_Key << ", " << m_RepeatCount;
            return ss.str();
        }
    private:
        int m_RepeatCount;
    };

    class KeyReleasedEvent : public KeyEvent
    {
    public:
        KeyReleasedEvent(int key)
            :KeyEvent(key) {}

        EVENT_CLASS_TYPE(KeyReleased)

        std::string ToString() const override
        {
            std::stringstream ss;
            ss << GetName() << ": " << m_Key;
            return ss.str();
        }
    };

}
