#include "Platform.h"

#include <GLFW/glfw3.h>

double Platform::GetTimeSec()
{
    return glfwGetTime();
}