#pragma once

#include <GLFW/glfw3.h>
#include <glm/glm.hpp>

class Input
{
public:
    static void Init(GLFWwindow *windowHandle);

    static bool IsKeyJustPressed(int keycode);
    static bool IsKeyJustReleased(int keycode);
    static bool IsKeyPressed(int keycode);

    static bool IsMouseButtonReleased(int button);
    static bool IsMouseButtonPressed(int button);
    static glm::vec2 GetMousePosition();

private:
    static GLFWwindow *s_WindowHandle;
};