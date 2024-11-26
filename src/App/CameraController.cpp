#include "CameraController.h"

#include "Core/Input.h"
#include "Core/Application.h"

namespace App
{
    CameraController::CameraController()
        :m_Camera(45.0f, 0.1f, 100.0f)
    {
        m_Camera.transform.SetPosition(glm::vec3(0.0f, 0.0f, 2.0f));
    }

    void CameraController::OnAttach()
    {
        auto [x, y] = Input::GetMousePosition();
        m_LastMousePos = glm::vec2(x, y);
    }

    void CameraController::OnUpdate(Timestep dt)
    {
        auto [x, y] = Input::GetMousePosition();
        glm::vec2 mousePos(x, y);

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
}