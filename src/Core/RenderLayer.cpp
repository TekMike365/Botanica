#include "RenderLayer.h"

#include "Renderer/Renderer.h"
#include "Renderer/RenderCommand.h"

namespace Botanica
{
    RenderLayer::RenderLayer()
        :Layer("Render Layer")
    {
    }

    RenderLayer::~RenderLayer()
    {
    }

    void RenderLayer::OnUpdate(Timestep dt)
    {
        Renderer::ExecuteRenderPipeline();

        if (Renderer::RenderCommand::IsRendering())
            BT_CORE_ASSERT(false, "IMPOSSIBLE!")

        Renderer::RenderCommand::Execute();
    }
}