#pragma once

#include "btpch.h"
#include "Camera.h"

#include "Events/Event.h"
#include "Events/KeyboardEvent.h"
#include "Events/MouseEvent.h"
#include "Events/ApplicationEvent.h"

#include <glm/glm.hpp>

namespace Botanica
{

    class Player
    {
    public:
        Player(float aspect);
        ~Player();

        void OnEvent(Event& e);
        void OnUpdate(float deltaTime);
        void OnEnable(float mouseXNormalized, float mouseYNormalized);

        inline const Camera& GetCamera() const { return m_Camera; }
    private:
        bool OnKeyPressed(KeyPressedEvent& e);
        bool OnKeyReleased(KeyReleasedEvent& e);
        bool OnMouseMoved(MouseMovedEvent& e);
        bool OnWindowResized(WindowResizeEvent& e);
    private:
        glm::vec3 m_MoveDir;
        glm::vec2 m_LastMousePosNormalized;
        glm::vec2 m_Rotate;

        bool m_Operate = false;

        CameraParams m_CameraParams;
        Camera m_Camera;
    };

}
