#include "btpch.h"

#include "Core/Application.h"
#include "App/MainLayer.h"

int main()
{
    Botanica::Log::Init();

    auto* app = new Botanica::Application();
    app->PushLayer(new App::MainLayer);
    app->Run();
    delete app;
}