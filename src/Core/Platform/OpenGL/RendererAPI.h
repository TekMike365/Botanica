#pragma once

#include <vector>

#include "Core/Renderer/RendererAPI.h"
#include "Core/Renderer/VertexArray.h"

namespace Botanica::Renderer::OpenGL
{
    enum class CommandType
    {
        None = 0, AdvanceState, DrawIndexed, ClearScreen
    };

    struct Command
    {
        CommandType Type = CommandType::None;
        std::vector<Uniform> Uniforms;
        size_t Count = 0;
        size_t Offset = 0;

        Command() = default;
        Command(CommandType type)
            : Type(type) {}
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

        virtual void DrawIndexed(size_t count, size_t offset) override;
        virtual void ClearScreen() override;

        virtual void Execute() override;

        virtual inline bool IsRendering() const override { return m_IsRendering; }
    
    private:
        bool m_IsRendering = false;
        std::vector<Uniform> m_DrawUniforms;
        CommandBuffer *m_PushBuffer;
        CommandBuffer *m_RenderBuffer;
        CommandBuffer m_BufferA;
        CommandBuffer m_BufferB;
    };
}