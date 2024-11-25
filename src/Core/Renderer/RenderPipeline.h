#pragma once

#include <initializer_list>
#include <functional>

#include "RendererAPI.h"

namespace Botanica::Renderer
{
    class RenderPass
    {
    public:
        virtual void Execute() = 0;
    };

    class RenderPipeline
    {
    public:
        RenderPipeline() = default;
        ~RenderPipeline() = default;

        RenderPipeline(std::initializer_list<std::shared_ptr<RenderPass>> renderPasses);

        void AddPass(const std::shared_ptr<RenderPass> &pass);
        void Execute();

    private:
        std::vector<std::shared_ptr<RenderPass>> m_RenderPasses;
    };
}