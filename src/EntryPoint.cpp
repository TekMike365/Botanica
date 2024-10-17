#include "btpch.h"

#include "Log.h"
#include "Core/Application.h"

int main()
{
    Botanica::Log::Init();

    auto* app = new Botanica::Application();
    app->Run();
    delete app;
}