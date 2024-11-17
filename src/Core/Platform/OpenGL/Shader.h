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

        void UploadUniform(const std::string &name, const glm::mat4 &mat) const override;
    
    private:
        uint32_t m_ID;
        std::unordered_map<std::string, int> m_UniformNameLocationMap;
    };
}
