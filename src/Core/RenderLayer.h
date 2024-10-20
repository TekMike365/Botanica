#pragma once

#include "Layer.h"

namespace Botanica
{
    class RenderLayer : public Layer
    {
    public:
        RenderLayer();
        ~RenderLayer();

        virtual void OnUpdate();
    };
}
