#include "Log.h"
#include "Application.h"

int main()
{
    Botanica::Core::Log::Init();

    auto* app = new Botanica::Core::Application();
    app->Run();
    delete app;
}