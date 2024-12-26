#pragma once

#include "Timestep.h"
#include "Event/Event.h"

class Layer
{
public:
    virtual void OnAttach() {}
    virtual void OnDetach() {}
    virtual void OnUpdate(Timestep dt) {}
    virtual void OnEvent(Event &e) {}
};