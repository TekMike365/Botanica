#pragma once

#include <cstdint>

#include "Core/Renderer/Shader.h"

namespace Botanica::OpenGL
{
    class Shader : public Botanica::Shader
    {
    public:
        Shader();
        ~Shader();
        
        void Bind() const override;
        void Unbind() const override;

        void AddSource(ShaderSourceType type, const std::string &source) override;

        void UploadUniform(const std::string &name, const glm::mat4 &mat) const override;
    
    private:
        uint32_t m_ID;
    };
}
