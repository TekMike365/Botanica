#include "btpch.h"
#include "CrossPlatformInput.h"

#include "GLFW/glfw3.h"

#include "Core/Application.h"

namespace Botanica
{
    Input *Input::s_Instance = new CrossPlatformInput();

    bool CrossPlatformInput::IsKeyJustPressedImpl(int keycode)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetGetNativeWindow());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS;
    }

    bool CrossPlatformInput::IsKeyJustReleasedImpl(int keycode)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetGetNativeWindow());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_RELEASE;
    }

    bool CrossPlatformInput::IsKeyPressedImpl(int keycode)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetGetNativeWindow());
        int state = glfwGetKey(window, keycode);
        return state == GLFW_PRESS || state == GLFW_REPEAT;
    }
    
    bool CrossPlatformInput::IsMouseButtonReleasedImpl(int button)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetGetNativeWindow());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_RELEASE;
    }
    
    bool CrossPlatformInput::IsMouseButtonPressedImpl(int button)
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetGetNativeWindow());
        int state = glfwGetMouseButton(window, button);
        return state == GLFW_PRESS;
    }
    
    std::pair<float, float> CrossPlatformInput::GetMousePositionImpl()
    {
        GLFWwindow *window = static_cast<GLFWwindow *>(Application::Get().GetWindow().GetGetNativeWindow());
        double xpos, ypos;
        glfwGetCursorPos(window, &xpos, &ypos);

        return { (float)xpos, (float)ypos };
    }
}