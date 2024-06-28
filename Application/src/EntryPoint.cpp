#include <stdio.h>

#include "Application.h"

int main()
{
    Botanica::Application* app = new Botanica::Application();
    app->Run();
    delete app;
}
