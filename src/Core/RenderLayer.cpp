#include "btpch.h"
#include "RenderLayer.h"

#include <glad/glad.h>

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

namespace Botanica
{
    RenderLayer::RenderLayer()
        :Layer("RenderLayer")
    {
    }
    
    RenderLayer::~RenderLayer()
    {
    }
    
    void RenderLayer::OnUpdate()
    {
        RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        RenderCommand::ClearScreen();
    }
}