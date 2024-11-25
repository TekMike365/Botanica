#include "Renderer.h"

namespace Botanica::Renderer
{
    static RenderPipeline s_Pipeline;

    void SetRenderPipeline(const RenderPipeline &pipeline)
    {
        s_Pipeline = pipeline;
    }

    void ExecuteRenderPipeline()
    {
        s_Pipeline.Execute();
    }
}