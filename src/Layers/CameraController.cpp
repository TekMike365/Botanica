#include "pch.h"
#include "CameraController.h"

#include "Input.h"
#include "Application.h"

CameraController::CameraController(const Camera &cam)
    :m_Camera(cam)
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
        if (Input::IsKeyPressed(GLFW_KEY_W))
            m_Camera.transform.Translate(m_Camera.transform.GetForwardVector() * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_S))
            m_Camera.transform.Translate(m_Camera.transform.GetForwardVector() * -speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_D))
            m_Camera.transform.Translate(m_Camera.transform.GetRightVector() * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_A))
            m_Camera.transform.Translate(m_Camera.transform.GetRightVector() * -speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_SPACE))
            m_Camera.transform.Translate(m_Camera.transform.GetUpVector() * speed * dt.GetSeconds());
        if (Input::IsKeyPressed(GLFW_KEY_LEFT_SHIFT))
            m_Camera.transform.Translate(m_Camera.transform.GetUpVector() * -speed * dt.GetSeconds());

        float angularSpeed = 3.141592f * 100.0f;
        glm::vec3 rotationDir = m_Camera.transform.GetRightVector() * mouseDir.y + glm::vec3(0.0f, 1.0f, 0.0f) * mouseDir.x;
        m_Camera.transform.Rotate(rotationDir * angularSpeed * dt.GetSeconds());
    }
}
