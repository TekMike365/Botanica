#include "Core/Platform.h"

#include <GLFW/glfw3.h>

namespace Botanica
{
    double Platform::GetTime()
    {
        return glfwGetTime();
    }
}