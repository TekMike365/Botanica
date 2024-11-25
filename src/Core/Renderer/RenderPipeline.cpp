#include "RenderPipeline.h"

namespace Botanica::Renderer
{
    RenderPipeline::RenderPipeline(std::initializer_list<std::shared_ptr<RenderPass>> renderPasses)
        : m_RenderPasses(renderPasses) {}

    void RenderPipeline::AddPass(const std::shared_ptr<RenderPass> &pass)
    {
        m_RenderPasses.push_back(pass);
    }

    void RenderPipeline::Execute()
    {
        for (auto &pass : m_RenderPasses)
            pass->Execute();
    }
}