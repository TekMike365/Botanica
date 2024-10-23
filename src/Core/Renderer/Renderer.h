#pragma once

#include "Shader.h"
#include "VertexArray.h"
#include "Camera.h"

namespace Botanica
{
    class Renderer
    {
    public:
        static void BeginScene(std::shared_ptr<Camera> camera);
        static void Submit(std::shared_ptr<Shader> shader, std::shared_ptr<VertexArray> vertexArray);
        static void EndScene();

    private:
        static std::shared_ptr<Camera> s_Camera;
    };
}