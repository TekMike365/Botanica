#pragma once

#include "Layer.h"
#include "Components/Camera.h"

class CameraController : public Layer
{
public:
    CameraController(const Camera &cam);

    virtual void OnAttach() override;
    virtual void OnUpdate(Timestep dt) override;

    inline Camera &GetCamera() { return m_Camera; }

private:
    Camera m_Camera;
    glm::vec2 m_LastMousePos;
};
