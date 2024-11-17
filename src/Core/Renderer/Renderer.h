#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

namespace Botanica::Renderer
{
    void BeginScene(std::shared_ptr<Camera> camera);
    void Submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray);
    void EndScene();
}