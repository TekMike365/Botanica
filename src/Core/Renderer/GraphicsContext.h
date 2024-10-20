#pragma once
#include "btpch.h"

namespace Botanica
{
    class GraphicsContext
    {
    public:
        virtual void Init() = 0;
        virtual void SwapBuffers() = 0;
    };
}