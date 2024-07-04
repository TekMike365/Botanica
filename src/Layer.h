#pragma once

#include "Events/Event.h"

namespace Botanica
{

    class Layer
    {
    public:
        virtual ~Layer() {}

        virtual void OnAttach() {}
        virtual void OnDetach() {}
        virtual void OnUpdate() {}
        virtual void OnEvent(Event& e) {}
    protected:
        Layer() = default;
    };

}
