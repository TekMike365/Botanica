#include "RenderLayer.h"

namespace Botanica
{
    RenderLayer::RenderLayer()
        :Layer("Render Layer")
    {
        // TODO: Setup Renderer
    }

    RenderLayer::~RenderLayer()
    {
    }

    void RenderLayer::OnUpdate(Timestep dt)
    {
        // TODO: Flush command buffer.
    }
}