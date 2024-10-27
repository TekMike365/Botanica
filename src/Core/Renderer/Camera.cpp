#include "btpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Botanica
{
    Camera::Camera(float fovDeg, float zNear, float zFar)
        : m_ViewMat(1.0)
    {
        float aspect = 1280.0f / 720.0f;
        m_ProjectionMat = glm::perspective(glm::radians(fovDeg), aspect, zNear, zFar);
    }

    Camera::~Camera()
    {
    }

    void Camera::RecalculateViewMat()
    {
        glm::mat4 transform = glm::translate(glm::mat4(1.0), m_Position) *
                              glm::rotate(glm::mat4(1.0), m_RotationY, glm::vec3(0, 1, 0));
        m_ViewMat = glm::inverse(transform);
        m_VPMat = m_ProjectionMat * m_ViewMat;
    }
}
