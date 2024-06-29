#include "btpch.h"
#include "Application.h"

#include "GLFW/glfw3.h"

namespace Botanica
{

    Application::Application() {}

    Application::~Application() {}

    void Application::Run()
    {
        BT_INFO("Hello, Botanica!");

        GLFWwindow* window;

        /* Initialize the library */
        if (!glfwInit())
        {
            BT_ERROR("GLFW failed to initialize.");
            return;
        }

        /* Create a windowed mode window and its OpenGL context */
        window = glfwCreateWindow(640, 480, "Hello World", NULL, NULL);
        if (!window)
        {
            BT_ERROR("GLFW failed to create window.");
            glfwTerminate();
            return;
        }

        /* Make the window's context current */
        glfwMakeContextCurrent(window);

        /* Loop until the user closes the window */
        while (!glfwWindowShouldClose(window))
        {
            /* Render here */
            glClearColor(0, 0.6, 0.9, 1);
            glClear(GL_COLOR_BUFFER_BIT);

            /* Swap front and back buffers */
            glfwSwapBuffers(window);

            /* Poll for and process events */
            glfwPollEvents();
        }

        glfwTerminate();
    }

}
