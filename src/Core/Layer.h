#pragma once
#include "btpch.h"

#include "Event/Event.h"

namespace Botanica
{
    class Layer
    {
    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event &e) {}

    private:
        std::string m_DebugName;
    };
}
