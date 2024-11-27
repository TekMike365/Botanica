#pragma once

#include <vector>

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/VertexArray.h"

namespace Botanica::Renderer::OpenGL
{
    enum class CommandType
    {
        None = 0, AdvanceState, DrawIndexed, ClearScreen, DispatchCompute
    };

    struct Command
    {
        CommandType Type = CommandType::None;
        std::vector<Uniform> Uniforms;
        std::vector<UniformBuffer> UniformBuffers;
        size_t Count = 0;
        size_t Offset = 0;
        uint32_t WorkGroups_x = 0;
        uint32_t WorkGroups_y = 0;
        uint32_t WorkGroups_z = 0;

        Command() = default;
        Command(CommandType type)
            : Type(type) {}
        Command(CommandType type, const std::vector<UniformBuffer> &uniformBuffers, uint32_t workGroups_x, uint32_t workGroups_y, uint32_t workGroups_z)
            : Type(type), WorkGroups_x(workGroups_x), WorkGroups_y(workGroups_y), WorkGroups_z(workGroups_z) {}
        Command(CommandType type, const std::vector<Uniform> &uniforms, size_t count = 0, size_t offset = 0)
            : Type(type), Uniforms(uniforms), Count(count), Offset(offset) {}
    };

    struct CommandBuffer
    {
        std::vector<RenderState> States;
        std::vector<Command> Commands;
    };

    class RendererAPI : public Renderer::RendererAPI
    {
    public:
        RendererAPI();
        ~RendererAPI() = default;

        virtual void SetRenderState(const RenderState& state) override;
        virtual void SetShaderUniforms(const std::vector<Uniform> &uniforms) override;
        virtual void SetShaderUniformBuffers(const std::vector<UniformBuffer> &uniformBuffers) override;

        virtual void DispatchCompute(uint32_t groups_x, uint32_t groups_y, uint32_t groups_z) override;
        virtual void DrawIndexed(size_t count, size_t offset) override;
        virtual void ClearScreen() override;

        virtual void Execute() override;

        virtual inline bool IsRendering() const override { return m_IsRendering; }
    
    private:
        bool m_IsRendering = false;
        std::vector<Uniform> m_DrawUniforms;
        std::vector<UniformBuffer> m_DrawUniformBuffers;
        CommandBuffer *m_PushBuffer;
        CommandBuffer *m_RenderBuffer;
        CommandBuffer m_BufferA;
        CommandBuffer m_BufferB;
    };
}