#pragma once

#include <cstdint>
#include <unordered_map>

#include "Core/Renderer/Shader.h"

namespace Botanica::Renderer::OpenGL
{
    class ShaderSource : public Renderer::ShaderSource
    {
        friend class Shader;
    public:
        ShaderSource(ShaderSourceType type, const std::string &source);
        ~ShaderSource();

        virtual inline ShaderSourceType GetType() const override { return m_Type; }

    private:
        ShaderSourceType m_Type;
        uint32_t m_ID;
    };

    class Shader : public Renderer::Shader
    {
    public:
        Shader(const ShaderSourceSPtrVec &sources);
        ~Shader();
        
        void Bind() const override;
        void Unbind() const override;

        virtual void UploadUniform(const Uniform& uniform) const override;
        virtual void UploadUniformBuffer(const std::string &name, std::shared_ptr<Buffer> ub, uint32_t bindingPoint) const override;

    private:
        void StoreUniformNameLocations();
        void StoreUniformBufferNameLocations();

    private:
        uint32_t m_ID;
        std::unordered_map<std::string, int> m_UniformNameLocationMap;
        std::unordered_map<std::string, int> m_UniformBufferNameLocationMap;
    };
}
