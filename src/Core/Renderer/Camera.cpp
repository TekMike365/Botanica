#include "btpch.h"
#include "Camera.h"

#include <glm/gtc/matrix_transform.hpp>

namespace Botanica
{
    Camera::Camera(float fovDeg, float zNear, float zFar)
    {
        glm::mat4 view(1.0);
        
        float aspect = 1280.0f / 720.0f;
        glm::mat4 proj = glm::perspective(glm::radians(fovDeg), aspect, zNear, zFar);

        m_VP = proj * view;
    }

    Camera::~Camera()
    {
    }
}
