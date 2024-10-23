#pragma once

#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Botanica
{
    class Camera
    {
    public:
        Camera(float fovDeg, float zNear, float zFar);
        ~Camera();

        inline void Translate(const glm::vec3 &v) { m_VP = glm::translate(m_VP, v); }
        inline void Scale(const glm::vec3 &v) { m_VP = glm::scale(m_VP, v); }
        inline void Rotate(float angleRad, const glm::vec3 &v) { m_VP = glm::rotate(m_VP, angleRad, v); }

        inline const glm::mat4 &GetVP() const { return m_VP; }

    private:
        glm::mat4 m_VP;
    };
}
