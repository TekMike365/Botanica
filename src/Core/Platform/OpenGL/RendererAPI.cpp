#include "btpch.h"
#include "RendererAPI.h"

#include <glad/glad.h>

namespace Botanica::Renderer
{
    RendererAPI *RendererAPI::Create()
    {
        return new OpenGL::RendererAPI();
    }
}

namespace Botanica::Renderer::OpenGL
{
    void OpenGL::RendererAPI::SetRenderState(const RenderState &state)
    {
        m_States.push_back(state);
        m_Commands.emplace_back(CommandType::AdvanceState);
    }

    void OpenGL::RendererAPI::SetShaderUniforms(const std::vector<Uniform> &uniforms)
    {
        m_DrawUniforms.insert(m_DrawUniforms.begin(), uniforms.begin(), uniforms.end());
    }

    void RendererAPI::DrawIndexed(size_t count, size_t offset)
    {
        m_Commands.emplace_back(CommandType::DrawIndexed, m_DrawUniforms, count, offset);
        m_DrawUniforms.resize(0);
    }

    void RendererAPI::ClearScreen()
    {
        m_Commands.emplace_back(CommandType::ClearScreen);
    }

    void OpenGL::RendererAPI::Execute()
    {
        auto stateIt = m_States.begin() - 1;
        for (const Command &command : m_Commands)
        {
            switch (command.Type)
            {
            case CommandType::None:
                BT_CORE_ASSERT(false, "CommandType cannot be None!")
                continue;
            case CommandType::AdvanceState:
            {
                stateIt++;
                stateIt->VertexArrayPtr->Bind();
                stateIt->ShaderPtr->Bind();
                continue;
            }
            case CommandType::ClearScreen:
            {
                glClearColor(stateIt->ClearColor.r, stateIt->ClearColor.g, stateIt->ClearColor.b, stateIt->ClearColor.a);
                glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
                continue;
            }
            case CommandType::DrawIndexed:
            {
                for (const auto &uniform : command.Uniforms)
                    stateIt->ShaderPtr->UploadUniform(uniform);
                glDrawElements(GL_TRIANGLES, command.Count, GL_UNSIGNED_INT, (const void *)command.Offset);
                continue;
            }
            }
            BT_CORE_ASSERT(false, "Unknown CommandType.")
        }

        m_Commands.resize(0);
        m_States.resize(0);
    }
}