#include "Application.h"

int main()
{
    auto* app = new Botanica::Core::Application();
    app->Run();
    delete app;
}