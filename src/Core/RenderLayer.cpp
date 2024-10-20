#include "btpch.h"
#include "RenderLayer.h"

#include <glad/glad.h>

#include "Renderer/RenderCommand.h"
#include "Renderer/Renderer.h"

const float VERTICES[] = {
    0.0f, 0.5f, 0.0f,
    -0.5f, -0.5f, 0.0f,
    0.5f, -0.5f, 0.0f};

const uint32_t INDICES[] = {0, 1, 2};

const char *VERTEX_SHADER_SOURCE = R"(
    #version 330 core
    layout (location = 0) in a_Position;

    int main()
    {
        gl_Position = vec4(a_Position, 1.0);
    }
)";


const char *FRAGMENT_SHADER_SOURCE = R"(
    #version 330 core
    out vec4 color;

    int main()
    {
        color = vec4(1.0, 0.5, 0.2, 1.0);
    }
)";

namespace Botanica
{
    RenderLayer::RenderLayer()
        : Layer("RenderLayer")
    {
        m_VertexBuffer.reset(VertexBuffer::Create(sizeof(VERTICES), VERTICES));
        m_IndexBuffer.reset(IndexBuffer::Create(sizeof(INDICES), INDICES));
        m_VertexArray.reset(VertexArray::Create(*m_VertexBuffer, *m_IndexBuffer));

        m_Shader.reset(Shader::Create());
        m_Shader->AddSource(ShaderType::Vertex, VERTEX_SHADER_SOURCE);
        m_Shader->AddSource(ShaderType::Fragment, FRAGMENT_SHADER_SOURCE);
    }

    RenderLayer::~RenderLayer()
    {
    }

    void RenderLayer::OnUpdate()
    {
        RenderCommand::SetClearColor(glm::vec4(0.2f, 0.3f, 0.3f, 1.0f));
        RenderCommand::ClearScreen();

        Renderer::BeginScene();
        Renderer::Submit(*m_Shader, *m_VertexArray);
        Renderer::EndScene();
    }
}