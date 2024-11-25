#pragma once

#include <initializer_list>
#include <functional>

#include "RendererAPI.h"

namespace Botanica::Renderer
{
    class RenderPipeline
    {
        using PassFn = std::function<void()>;
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        RenderPipeline(std::initializer_list<PassFn> renderPasses);

        void AddPass(PassFn pass);
        void Execute();

    private:
        std::vector<PassFn> m_RenderPasses;
    };
}