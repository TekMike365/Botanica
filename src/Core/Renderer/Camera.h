#pragma once

#include "Core/Transform.h"

namespace Botanica
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

        glm::mat4 GetVPMat() const { return m_ProjectionMat * transform.GetInverseMatrix(); }

    public:
        Transform transform;

    private:
        glm::mat4 m_ProjectionMat;
    };
}
