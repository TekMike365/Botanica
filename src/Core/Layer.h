#pragma once
#include "btpch.h"

#include <string>

#include "Event/Event.h"
#include "Timestep.h"

namespace Botanica
{
    class Layer
    {
    public:
        Layer(const std::string &name = "Layer");
        virtual ~Layer();

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate(Timestep dt) {}
        virtual void OnEvent(Event &e) {}

        const std::string &GetName() const { return m_DebugName; }

    private:
        std::string m_DebugName;
    };
}
