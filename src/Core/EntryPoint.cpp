#include "Log.h"
#include "Application.h"

int main()
{
    Botanica::Log::Init();

    auto* app = new Botanica::Application();
    app->Run();
    delete app;
}