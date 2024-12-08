#pragma once

#include <GLFW/glfw3.h>

namespace Renderer
{
    void Init(GLFWwindow *windowHandle);
    void SwapBuffers();
}