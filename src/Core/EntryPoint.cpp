#include "btpch.h"
#include "Application.h"

int main()
{
    Botanica::Log::Init(); // temp

    Botanica::Application* app = new Botanica::Application();
    app->Run();
    delete app;
}
