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

        inline const glm::vec3 &GetPosition() const { return m_Position; }
        inline void SetPosition(const glm::vec3 &position)
        {
            m_Position = position;
            RecalculateViewMat();
        }

        inline float GetRotationY() const { return m_RotationY; }
        inline void SetRotationY(float rotation)
        {
            m_RotationY = rotation;
            RecalculateViewMat();
        }

        inline const glm::mat4 &GetVPMat() const { return m_VPMat; }

    private:
        void RecalculateViewMat();

    private:
        glm::mat4 m_ProjectionMat;
        glm::mat4 m_ViewMat;
        glm::mat4 m_VPMat;

        glm::vec3 m_Position;
        float m_RotationY;
    };
}
