#include "RenderPipeline.h"

namespace Botanica::Renderer
{
    RenderPipeline::RenderPipeline(std::initializer_list<PassFn> renderPasses)
        : m_RenderPasses(renderPasses) {}

    void RenderPipeline::AddPass(PassFn pass)
    {
        m_RenderPasses.push_back(pass);
    }

    void RenderPipeline::Execute()
    {
        for (PassFn exec : m_RenderPasses)
            exec();
    }
}