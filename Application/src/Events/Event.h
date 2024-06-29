#pragma once

#include "btpch.h"
#include "Core.h"

namespace Botanica
{

    enum class EventType
    {
        None = 0,
        WindowClose, WindowResize
    };

    enum EventCategory
    {
        None = 0,
        EventCategoryApplication = BIT(0)
    };

#define EVENT_CLASS_TYPE(type) inline static EventType GetStaticType() { return EventType::type; }\
                                inline EventType GetEventType() const { return GetStaticType(); }\
                                inline const char* GetName() const override { return #type; } 

#define EVENT_CLASS_CATEGORY(category) EventCategory GetCategoryFlags() const override { return category; }

    class Event
    {
        friend class EventDispatcher;
    public:
        virtual EventType GetEventType() const  = 0;
        virtual EventCategory GetCategoryFlags() const = 0;
        virtual const char* GetName() const = 0;
        virtual std::string ToString() const { return GetName(); }

        bool IsInCategory(EventCategory category)
        {
            return GetCategoryFlags() & category;
        }

    protected:
        bool m_Handled;
    };

    class EventDispatcher
    {
        template<typename T>
        using EventFn = std::function<bool(T&)>;
    public:
        EventDispatcher(Event& event)
            :m_Event(event) {}

        template<typename T>
        bool Dispatch(EventFn<T> func)
        {
            if (m_Event.GetEventType() == T::GetStaticType())
            {
                m_Event.m_Handled = func(*(T*)&m_Event);
                return true;
            }
            return false;
        }

    private:
        Event& m_Event;
    };

}
