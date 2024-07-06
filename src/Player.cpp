#include "Player.h"

#include <GLFW/glfw3.h>

namespace Botanica
{

    Player::Player(float aspect)
        :m_CameraParams(45.0f, aspect, 0.4f, 100.0f), m_Camera(m_CameraParams), m_MoveDir(0.0f), m_MousePosNormalized(0.5f)
    {
    }

    Player::~Player()
    {
    }

    void Player::OnEvent(Event &e)
    {
        EventDispatcher dispatcher(e);
        dispatcher.Dispatch<KeyPressedEvent>(BIND_EVENT_FN(Player::OnKeyPressed));
        dispatcher.Dispatch<KeyReleasedEvent>(BIND_EVENT_FN(Player::OnKeyReleased));
        dispatcher.Dispatch<MouseMovedEvent>(BIND_EVENT_FN(Player::OnMouseMoved));
        dispatcher.Dispatch<WindowResizeEvent>(BIND_EVENT_FN(Player::OnWindowResized));
    }

    void Player::OnUpdate(float deltaTime)
    {
        float moveSpeed = 4.0f;
        m_Camera.Translate(m_MoveDir * moveSpeed * deltaTime);

        if (m_MousePosNormalized.y > 1.0f || m_MousePosNormalized.y < 0.0f)
            return;
        glm::vec2 rot = glm::vec2(m_MousePosNormalized.x, m_MousePosNormalized.y) * 180.0f - 90.0f;
        m_Camera.Rotate(-rot.y, rot.x);
    }

    bool Player::OnKeyPressed(KeyPressedEvent &e)
    {
        if (e.GetRepeatCount())
            return false;

        switch (e.GetKey())
        {
            case GLFW_KEY_W:
                m_MoveDir.z += 1.0f;
                return true;
            case GLFW_KEY_S:
                m_MoveDir.z += -1.0f;
                return true;
            case GLFW_KEY_D:
                m_MoveDir.x += 1.0f;
                return true;
            case GLFW_KEY_A:
                m_MoveDir.x += -1.0f;
                return true;
            case GLFW_KEY_SPACE:
                m_MoveDir.y += 1.0f;
                return true;
            case GLFW_KEY_RIGHT_SHIFT:
            case GLFW_KEY_LEFT_SHIFT:
                m_MoveDir.y += -1.0f;
                return true;
            case GLFW_KEY_F3:
                m_Operate = true;
                return true;
        }

        return false;
    }

    bool Player::OnKeyReleased(KeyReleasedEvent &e)
    {
        switch (e.GetKey())
        {
            case GLFW_KEY_W:
                m_MoveDir.z -= 1.0f;
                return true;
            case GLFW_KEY_S:
                m_MoveDir.z -= -1.0f;
                return true;
            case GLFW_KEY_D:
                m_MoveDir.x -= 1.0f;
                return true;
            case GLFW_KEY_A:
                m_MoveDir.x -= -1.0f;
                return true;
            case GLFW_KEY_SPACE:
                m_MoveDir.y -= 1.0f;
                return true;
            case GLFW_KEY_RIGHT_SHIFT:
            case GLFW_KEY_LEFT_SHIFT:
                m_MoveDir.y -= -1.0f;
                return true;
        }

        return false;
    }

    bool Player::OnMouseMoved(MouseMovedEvent &e)
    {
        m_MousePosNormalized.x = e.GetXPosNormalized();
        m_MousePosNormalized.y = e.GetYPosNormalized();
        return true;
    }

    bool Player::OnWindowResized(WindowResizeEvent &e)
    {
        m_CameraParams.Aspect = e.GetWidth() / e.GetHeight();
        m_Camera.UpdateParams(m_CameraParams);
        return true;
    }

}
