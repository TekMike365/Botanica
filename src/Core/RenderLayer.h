#pragma once

#include "Layer.h"

namespace Botanica
{
    class RenderLayer : public Layer
    {
    public:
        RenderLayer();
        virtual ~RenderLayer() override;

        virtual void OnUpdate(Timestep dt) override;
    };
}