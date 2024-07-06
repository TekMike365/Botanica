#pragma once

#include "btpch.h"
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>

namespace Botanica
{

    struct CameraParams
    {
        float FOV;
        float Aspect;
        float NearPlane;
        float FarPlane;

        CameraParams(float fov = 45.0f, float aspect = 1080.0f / 720.0f, float nearPlane = 0.4f, float farPlane = 100.0f)
            :FOV(fov), Aspect(aspect), NearPlane(nearPlane), FarPlane(farPlane)
        {
        }
    };

    class Camera
    {
    public:
        Camera(const CameraParams& params = CameraParams())
            :m_Params(params), Position(0.0f), m_Forward(0.0f, 0.0f, -1.0f)
        {
            UpdateProjection();
        }

        inline void UpdateParams(const CameraParams& params) { m_Params = params; UpdateProjection(); }
        inline const CameraParams& GetParams() const { return m_Params; };

        inline void Translate(glm::vec3 step)
        {
            glm::vec3 right = glm::cross(m_Forward, m_Up);
            Position += step.x * glm::normalize(right - m_Up * right)
                      + step.y * glm::normalize(m_Up)
                      + step.z * glm::normalize(m_Forward - m_Up * m_Forward);
        }
        inline void Rotate(float pitchDeg, float yawDeg)
        {
            yawDeg -= 90.0f;
            m_Forward.x = cos(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
            m_Forward.y = sin(glm::radians(pitchDeg));
            m_Forward.z = sin(glm::radians(yawDeg)) * cos(glm::radians(pitchDeg));
        }

        inline glm::vec3 GetForward() const { return m_Forward; }
        inline glm::mat4 GetView() const { return glm::lookAt(Position, Position + m_Forward, m_Up); }
        inline glm::mat4 GetProjection() const { return m_Projection; }
    public:
        glm::vec3 Position;
    private:
        inline void UpdateProjection() { m_Projection = glm::perspective(m_Params.FOV, m_Params.Aspect, m_Params.NearPlane, m_Params.FarPlane); }
    private:
        glm::vec3 m_Up = glm::vec3(0.0f, 1.0f, 0.0f);
        glm::vec3 m_Forward;
        CameraParams m_Params;
        glm::mat4 m_Projection;
    };

}
