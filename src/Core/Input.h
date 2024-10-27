#pragma once

namespace Botanica
{
    class Input
    {
    public:
        inline static bool IsKeyJustPressed(int keycode) { return s_Instance->IsKeyJustPressedImpl(keycode); }
        inline static bool IsKeyJustReleased(int keycode) { return s_Instance->IsKeyJustReleasedImpl(keycode); }
        inline static bool IsKeyPressed(int keycode) { return s_Instance->IsKeyPressedImpl(keycode); }
        
        inline static bool IsMouseButtonPressed(int button) { return s_Instance->IsMouseButtonPressedImpl(button); }
        inline static bool IsMouseButtonReleased(int button) { return s_Instance->IsMouseButtonReleasedImpl(button); }
        inline static std::pair<float, float> GetMousePosition() { return s_Instance->GetMousePositionImpl(); }
        
    protected:
        virtual bool IsKeyJustPressedImpl(int keycode) = 0;
        virtual bool IsKeyJustReleasedImpl(int keycode) = 0;
        virtual bool IsKeyPressedImpl(int keycode) = 0;

        virtual bool IsMouseButtonReleasedImpl(int button) = 0;
        virtual bool IsMouseButtonPressedImpl(int button) = 0;
        virtual std::pair<float, float> GetMousePositionImpl () = 0;

    private:
        static Input *s_Instance;
    };
}
