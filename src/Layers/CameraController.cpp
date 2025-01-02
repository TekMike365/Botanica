#include "pch.h"
#include "CameraController.h"

#include "Input.h"
#include "Application.h"

float Clamp(float val, float min, float max)
{
    if (val <= min)
        return min;
    if (val >= max)
        return max;
    return val;
}

CameraController::CameraController(Camera &cam)
    : m_Camera(cam)
{
}

void CameraController::OnAttach()
{
    m_LastMousePos = Input::GetMousePosition();
}

void CameraController::OnUpdate(Timestep dt)
{
    glm::vec2 mousePos = Input::GetMousePosition();

    glm::vec2 mouseDir = m_LastMousePos - mousePos;
    mouseDir.x /= Application::Get().GetWindow().GetWidth();
    mouseDir.y /= Application::Get().GetWindow().GetHeight();

    m_LastMousePos = mousePos;

    if (Input::IsMouseButtonPressed(GLFW_MOUSE_BUTTON_RIGHT))
    {
        float speed = 4.0f;

        glm::vec3 camFwdXZ = m_Camera.transform.GetForwardVector();
        camFwdXZ.y = 0;
        camFwdXZ = glm::normalize(camFwdXZ);

        glm::vec3 camRightXZ = m_Camera.transform.GetRightVector();
        camRightXZ.y = 0;
        camRightXZ = glm::normalize(camRightXZ);

        if (Input::IsKeyPressed(GLFW_KEY_W))
            m_Camera.transform.Translate(camFwdXZ * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_S))
            m_Camera.transform.Translate(camFwdXZ * -speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_D))
            m_Camera.transform.Translate(camRightXZ * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_A))
            m_Camera.transform.Translate(camRightXZ * -speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            m_Camera.transform.Translate(glm::vec3(0.0f, 1.0f, 0.0f) * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
            m_Camera.transform.Translate(glm::vec3(0.0f, 1.0f, 0.0f) * -speed * dt.GetSeconds());

        float angularSpeed = 3.141592f * 100.0f;

        m_YawRad += mouseDir.x * angularSpeed * dt.GetSeconds();
        m_PitchRad += mouseDir.y * angularSpeed * dt.GetSeconds();
        m_PitchRad = Clamp(m_PitchRad, glm::radians(-89.0f), glm::radians(89.0f));

        glm::quat yaw((float)cos(m_YawRad / 2.0f), (float)sin(m_YawRad / 2.0f) * glm::vec3(0.0f, 1.0f, 0.0f));
        m_Camera.transform.SetRotation(yaw);

        glm::quat pitch((float)cos(m_PitchRad / 2.0f), (float)sin(m_PitchRad / 2.0f) * m_Camera.transform.GetRightVector());
        m_Camera.transform.SetRotation(pitch * yaw);
    }
}
