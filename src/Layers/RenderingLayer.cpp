#include "pch.h"
#include "RenderingLayer.h"

RenderingLayer::RenderingLayer(Camera &cam, World<uint32_t> &world)
    : m_Camera(cam), m_World(world)
{
}

void RenderingLayer::OnAttach()
{
    const char *vert = R"(
        #version 430 core
        layout (location = 0) in vec3 aPos;
        layout (location = 1) in vec4 aCol;

        uniform mat4 uVP;

        out vec4 vCol;

        void main()
        {
            gl_Position = uVP * vec4(aPos.x, aPos.y, aPos.z, 1.0f);
            vCol = aCol;
        }
    )";

    const char *frag = R"(
        #version 430 core

        in vec4 vCol;

        out vec4 fColor;

        void main()
        {
            fColor = vCol;
        }
    )";

    struct Vertex
    {
        glm::vec3 Pos;
        glm::vec4 Col;
    };

    const Vertex vertices[] = {
        {glm::vec3(0.0f, 0.5f, 0.0f), glm::vec4(1.0f, 0.0f, 0.0f, 1.0f)},
        {glm::vec3(0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 1.0f, 0.0f, 1.0f)},
        {glm::vec3(-0.5f, -0.5f, 0.0f), glm::vec4(0.0f, 0.0f, 1.0f, 1.0f)}};

    const uint32_t indices[] = {0, 1, 2};

    Renderer::BufferLayout vbl({{Renderer::ShaderDataType::Float3},
                                {Renderer::ShaderDataType::Float4}});

    Renderer::BufferLayout ibl({{Renderer::ShaderDataType::UInt}});

    std::shared_ptr<Renderer::Buffer> vb = std::make_shared<Renderer::Buffer>(sizeof(vertices), (const void *)vertices);
    vb->SetLayout(vbl);
    std::shared_ptr<Renderer::Buffer> ib = std::make_shared<Renderer::Buffer>(sizeof(indices), (const void *)indices);
    ib->SetLayout(ibl);
    m_VertexArray = std::make_shared<Renderer::VertexArray>(vb, ib);

    Renderer::ShaderSource vertSrc(Renderer::ShaderSourceType::Vertex, vert);
    Renderer::ShaderSource fragSrc(Renderer::ShaderSourceType::Fragment, frag);
    m_Shader = std::shared_ptr<Renderer::Shader>(new Renderer::Shader({&vertSrc, &fragSrc}));
}

void RenderingLayer::OnUpdate(Timestep dt)
{
    Renderer::SetClearColor(glm::vec4(0.5f, 0.7f, 0.8f, 1.0f));
    Renderer::ClearScreen();

    m_VertexArray->Bind();
    m_Shader->Bind();
    m_Shader->UploadUniform(Renderer::UniformType::Mat4, "uVP", (const void *)&m_Camera.GetVPMat());

    Renderer::DrawIndexed(3);
}
