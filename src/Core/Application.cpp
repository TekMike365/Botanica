#include "cpch.h"

#include "Application.h"

namespace Botanica::Core
{
    Application::Application()
    {
    }

    Application::~Application()
    {
    }
    
    void Application::Run()
    {
        std::cout << "Runnig application." << std::endl;
    }
}