#pragma once

#include "Core/Input.h"

namespace Botanica
{
    class CrossPlatformInput : public Input
    {
    protected:
        virtual bool IsKeyJustPressedImpl(int keycode) override;
        virtual bool IsKeyJustReleasedImpl(int keycode) override;
        virtual bool IsKeyPressedImpl(int keycode) override;

        virtual bool IsMouseButtonReleasedImpl(int button) override;
        virtual bool IsMouseButtonPressedImpl(int button) override;
        virtual std::pair<float, float> GetMousePositionImpl () override;
    };
}
