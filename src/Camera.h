#pragma once

#include "btpch.h"
#include <glm/glm.hpp>

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
            :m_Params(params), Position(0.0f), Forward(glm::vec3(0.0f, 0.0f, -1.0f)), Up(glm::vec3(0.0f, 1.0f, 0.0f)), Right(glm::vec3(1.0f, 0.0f, 0.0f)) { UpdateProjection(); }

        inline void UpdateParams(const CameraParams& params) { m_Params = params; UpdateProjection(); }
        inline const CameraParams& GetParams() const { return m_Params; };

        inline void Move(glm::vec3 step) { Position += step * Forward + step * Up + step * Right; }
        inline void Rotate(float pitchDeg, float yawDeg, float rollDeg = 0) {
            Forward = RotateVec(Forward, pitchDeg, yawDeg, rollDeg);
            Up = RotateVec(Up, pitchDeg, yawDeg, rollDeg);
            Right = RotateVec(Right, pitchDeg, yawDeg, rollDeg);
        }

        inline glm::mat4 GetView() const { return glm::lookAt(Position, Position + Forward, Up); }
        inline glm::mat4 GetProjection() const { return m_Projection; }
    public:
        glm::vec3 Position;
        glm::vec3 Forward;
        glm::vec3 Up;
        glm::vec3 Right;
    private:
        glm::vec3 RotateVec(glm::vec3 vec, float pitchDeg, float yawDeg, float rollDeg) {
            glm::mat4 rotation(1.0f);
            rotation = glm::rotate(rotation, glm::radians(pitchDeg), Right);
            rotation = glm::rotate(rotation, glm::radians(yawDeg), Up);
            rotation = glm::rotate(rotation, glm::radians(rollDeg), Forward);
            return glm::vec3(glm::vec4(vec, 1.0f) * rotation);
        }
        inline void UpdateProjection() { m_Projection = glm::perspective(m_Params.FOV, m_Params.Aspect, m_Params.NearPlane, m_Params.FarPlane); }
    private:
        CameraParams m_Params;
        glm::mat4 m_Projection;
    };

}
