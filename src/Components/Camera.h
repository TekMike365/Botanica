#pragma once

#include "Transform.h"

class Camera
{
public:
    Camera(float fovDeg = 60, float aspect = 14.0f / 9.0f, float zNear = 0.1f, float zFar = 100.0f)
        : m_FovDeg(fovDeg), m_Aspect(aspect), m_ZNear(zNear), m_ZFar(zFar)
    {
        m_ProjectionMat = glm::perspective(glm::radians(fovDeg), aspect, zNear, zFar);
        transform.SetPosition(glm::vec3(0.0f, 0.0f, 0.0f));
    }
    ~Camera() = default;

    const glm::mat4 &GetVPMat()
    {
        m_MVPMat = m_ProjectionMat * transform.GetInverseMatrix();
        return m_MVPMat;
    }

    inline float GetFovDeg() const { return m_FovDeg; }
    inline float GetAspect() const { return m_Aspect; }
    inline float GetZNear() const { return m_ZNear; }
    inline float GetZFar() const { return m_ZFar; }

public:
    Transform transform;

private:
    glm::mat4 m_ProjectionMat;
    glm::mat4 m_MVPMat;

    float m_FovDeg;
    float m_Aspect;
    float m_ZNear;
    float m_ZFar;
};