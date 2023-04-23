#pragma once

#include "runtime/system/platform/win32/win32_input.hpp"

namespace engine {
    namespace input {

        enum class KeyCode {
            Q, W, E, R, T, Y, U, I, O, P,
            A, S, D, F, G, H, J, K, L,
            Z, X, C, V, B, N, M,
            Keypad1, Keypad2, Keypad3, Keypad4, Keypad5, Keypad6, Keypad7, Keypad8, Keypad9, Keypad0,
            UpArrow, DownArrow, LeftArrow, RightArrow,
            Space,
            Mouse0, Mouse1, Mouse2
        };

        class InputSystem final {
        public:
            InputSystem() = default;

        private:
            friend bool GetKey(KeyCode keyCode);
            friend bool GetKeyDown(KeyCode keyCode);
            friend bool GetKeyUp(KeyCode keyCode);

#ifdef _WIN32
            friend void Init(win32::KeyboardDevice* pKeyboardDevice, win32::MouseDevice* pMouseDevice);
            win32::KeyboardDevice* pKeyboardDevice;
            win32::MouseDevice* pMouseDevice;
#endif // _WIN32
        };

        InputSystem& GetInputSystem();

#ifdef _WIN32
        uint8_t Win32KeyCodeMapping(KeyCode keyCode);
        void Init(win32::KeyboardDevice* pKeyboardDevice, win32::MouseDevice* pMouseDevice);
#endif // _WIN32

        bool GetKey(KeyCode keyCode);
        bool GetKeyDown(KeyCode keyCode);
        bool GetKeyUp(KeyCode keyCode);

    }
}