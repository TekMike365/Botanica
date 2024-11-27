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
    OpenGL::RendererAPI::RendererAPI()
    {
        m_PushBuffer = &m_BufferA;
        m_RenderBuffer = &m_BufferB;
    }

    void OpenGL::RendererAPI::SetRenderState(const RenderState &state)
    {
        m_PushBuffer->States.push_back(state);
        m_PushBuffer->Commands.emplace_back(CommandType::AdvanceState);
    }

    void OpenGL::RendererAPI::SetShaderUniforms(const std::vector<Uniform> &uniforms)
    {
        m_DrawUniforms.insert(m_DrawUniforms.begin(), uniforms.begin(), uniforms.end());
    }

    void OpenGL::RendererAPI::SetShaderUniformBuffers(const std::vector<UniformBuffer> &uniformBuffers)
    {
        m_DrawUniformBuffers.insert(m_DrawUniformBuffers.begin(), uniformBuffers.begin(), uniformBuffers.end());
    }

    void OpenGL::RendererAPI::DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z)
    {
        m_PushBuffer->Commands.emplace_back(CommandType::DispatchCompute, m_DrawUniformBuffers, groups_x, groups_y, groups_z);
        m_DrawUniformBuffers.resize(0);
    }

    void RendererAPI::DrawIndexed(size_t count, size_t offset)
    {
        m_PushBuffer->Commands.emplace_back(CommandType::DrawIndexed, m_DrawUniforms, count, offset);
        m_DrawUniforms.resize(0);
    }

    void RendererAPI::ClearScreen()
    {
        m_PushBuffer->Commands.emplace_back(CommandType::ClearScreen);
    }

    void OpenGL::RendererAPI::Execute()
    {
        m_IsRendering = true;

        {
        CommandBuffer *hold = m_RenderBuffer;
        m_PushBuffer = m_RenderBuffer;
        m_RenderBuffer = hold;
        }

        auto stateIt = m_RenderBuffer->States.begin() - 1;
        for (const Command &command : m_RenderBuffer->Commands)
        {
            switch (command.Type)
            {
            case CommandType::None:
                BT_CORE_ASSERT(false, "CommandType cannot be None!")
                continue;
            case CommandType::AdvanceState:
            {
                stateIt++;
                if (stateIt->VertexArrayPtr)
                    stateIt->VertexArrayPtr->Bind();
                if (stateIt->ShaderPtr)
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
            case CommandType::DispatchCompute:
            {
                for (const auto &ub : command.UniformBuffers)
                    stateIt->ShaderPtr->UploadUniformBuffer(ub);
                glDispatchCompute(command.WorkGroups_x, command.WorkGroups_y, command.WorkGroups_z);
                // make sure writing to buffers has finished before read
                glMemoryBarrier(GL_ALL_BARRIER_BITS);
                continue;
            }
            }
            BT_CORE_ASSERT(false, "Unknown CommandType.")
        }

        m_RenderBuffer->Commands.resize(0);
        m_RenderBuffer->States.resize(0);

        m_IsRendering = false;
    }
}