#include <stdio.h>

#include "Application.h"
#include "Log.h"

int main()
{
    Botanica::Log::Init(); // temp

    Botanica::Application* app = new Botanica::Application();
    app->Run();
    delete app;
}
