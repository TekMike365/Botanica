#include "pch.h"
#include "Input.h"

GLFWwindow *Input::s_WindowHandle = nullptr;

void Input::Init(GLFWwindow *windowHandle)
{
    BT_ASSERT(!s_WindowHandle, "renderer initialised multiple times")
    s_WindowHandle = windowHandle;
}

bool Input::IsKeyJustPressed(int keycode)
{
    int state = glfwGetKey(s_WindowHandle, keycode);
    return state == GLFW_PRESS;
}

bool Input::IsKeyJustReleased(int keycode)
{
    int state = glfwGetKey(s_WindowHandle, keycode);
    return state == GLFW_RELEASE;
}

bool Input::IsKeyPressed(int keycode)
{
    int state = glfwGetKey(s_WindowHandle, keycode);
    return state == GLFW_PRESS || state == GLFW_REPEAT;
}

bool Input::IsMouseButtonReleased(int button)
{
    int state = glfwGetMouseButton(s_WindowHandle, button);
    return state == GLFW_RELEASE;
}

bool Input::IsMouseButtonPressed(int button)
{
    int state = glfwGetMouseButton(s_WindowHandle, button);
    return state == GLFW_PRESS;
}

glm::vec2 Input::GetMousePosition()
{
    double xpos, ypos;
    glfwGetCursorPos(s_WindowHandle, &xpos, &ypos);

    return glm::vec2((float)xpos, (float)ypos);
}
