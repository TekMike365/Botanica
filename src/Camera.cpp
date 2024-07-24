#include "Camera.h"

namespace Botanica
{

    Camera::Camera(const CameraParams& params)
        :m_Params(params), Position(0.0f), m_Forward(0.0f, 0.0f, -1.0f)
    {
        UpdateProjection();
    }

    void Camera::UpdateParams(const CameraParams& params)
    {
        m_Params = params;
        UpdateProjection();
    }

    void Camera::Translate(glm::vec3 step)
    {
        glm::vec3 right = glm::cross(m_Forward, m_Up);
        Position += step.x * glm::normalize(right - m_Up * right)
                    + step.y * glm::normalize(m_Up)
                    + step.z * glm::normalize(m_Forward - m_Up * m_Forward);
    }

    void Camera::Rotate(float pitchDeg, float yawDeg)
    {
        yawDeg -= 90.0f;
        m_Forward.x = cos(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
        m_Forward.y = sin(glm::radians(pitchDeg));
        m_Forward.z = sin(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
    }

}
