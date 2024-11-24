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
        Renderer::RenderCommand::Execute();
    }
}