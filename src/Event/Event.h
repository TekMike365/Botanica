#pragma once

#include <string>
#include <functional>

#include "Utils.h"

enum class EventType
{
    None = 0,
    WindowClose,
    WindowResize,
    WindowFocus,
    WindowLostFocus,
    KeyPressed,
    KeyReleased,
    MouseMoved,
    MouseButtonPressed,
    MouseButtonReleased,
    MouseScrolled
};

enum EventCategory
{
    None = 0,
    EventCategoryWindow = BIT(0),
    EventCategoryInput = BIT(1),
    EventCategoryButton = BIT(2),
    EventCategoryKeyboard = BIT(3),
    EventCategoryMouse = BIT(4)
};

#define EVENT_CLASS_TYPE(type)                                                    \
    static inline EventType GetStaticType() { return EventType::type; }           \
    virtual inline EventType GetType() const override { return GetStaticType(); } \
    virtual inline const char *GetName() const override { return #type; }

#define EVENT_CLASS_CATEGORY(category) \
    virtual inline int GetCategoryFlags() const override { return category; }

class Event
{
    friend class EventDispatcher;

public:
    virtual EventType GetType() const = 0;
    virtual int GetCategoryFlags() const = 0;

    virtual const char *GetName() const = 0;
    virtual inline std::string ToString() const { return GetName(); }

private:
    bool m_Handled = false;
};

class EventDispatcher
{
    template <typename T>
    using EventFn = std::function<bool(T &)>;

public:
    EventDispatcher(Event &e)
        : m_Event(e) {}

    template <typename T>
    bool Dispatch(EventFn<T> func)
    {
        if (T::GetStaticType() == m_Event.GetType())
        {
            m_Event.m_Handled = func(*(T *)&m_Event);
            return true;
        }

        return false;
    }

private:
    Event &m_Event;
};