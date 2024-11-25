#include "RenderLayer.h"

#include "Renderer/RenderCommand.h"

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
        if (Renderer::RenderCommand::IsRendering())
            BT_CORE_ASSERT(false, "IMPOSSIBLE!")

        Renderer::RenderCommand::Execute();
    }
}