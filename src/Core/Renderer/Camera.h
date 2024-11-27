#pragma once

#include "Core/Transform.h"

namespace Botanica::Renderer
{
    class Camera
    {
    public:
        Camera(float fovDeg, float zNear, float zFar)
        {
            float aspect = 1280.0f / 720.0f;
            m_ProjectionMat = glm::perspective(glm::radians(fovDeg), aspect, zNear, zFar);
        }
        ~Camera() {}

        const glm::mat4 &GetVPMat()
        {
            m_MVPMat = m_ProjectionMat * transform.GetInverseMatrix();
            return m_MVPMat;
        }

    public:
        Transform transform;

    private:
        glm::mat4 m_ProjectionMat;
        glm::mat4 m_MVPMat;
    };
}
