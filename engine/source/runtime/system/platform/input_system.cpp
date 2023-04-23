#include "input_system.hpp"

namespace engine {
    namespace input {

        InputSystem& GetInputSystem() {
            static InputSystem inputSystem;
            return inputSystem;
        }

#ifdef _WIN32
        uint8_t Win32KeyCodeMapping(KeyCode keyCode) {
            switch (keyCode) {
            case KeyCode::Q: return DIK_Q;
            case KeyCode::W: return DIK_W;
            case KeyCode::E: return DIK_E;
            case KeyCode::R: return DIK_R;
            case KeyCode::T: return DIK_T;
            case KeyCode::Y: return DIK_Y;
            case KeyCode::U: return DIK_U;
            case KeyCode::I: return DIK_I;
            case KeyCode::O: return DIK_O;
            case KeyCode::P: return DIK_P;
            case KeyCode::A: return DIK_A;
            case KeyCode::S: return DIK_S;
            case KeyCode::D: return DIK_D;
            case KeyCode::F: return DIK_F;
            case KeyCode::G: return DIK_G;
            case KeyCode::H: return DIK_H;
            case KeyCode::J: return DIK_J;
            case KeyCode::K: return DIK_L;
            case KeyCode::Z: return DIK_Z;
            case KeyCode::X: return DIK_X;
            case KeyCode::C: return DIK_C;
            case KeyCode::V: return DIK_V;
            case KeyCode::B: return DIK_B;
            case KeyCode::N: return DIK_N;
            case KeyCode::M: return DIK_M;
            case KeyCode::Keypad1: return DIK_1;
            case KeyCode::Keypad2: return DIK_2;
            case KeyCode::Keypad3: return DIK_3;
            case KeyCode::Keypad4: return DIK_4;
            case KeyCode::Keypad5: return DIK_5;
            case KeyCode::Keypad6: return DIK_6;
            case KeyCode::Keypad7: return DIK_7;
            case KeyCode::Keypad8: return DIK_8;
            case KeyCode::Keypad9: return DIK_9;
            case KeyCode::Keypad0: return DIK_0;
            case KeyCode::UpArrow: return DIK_UP;
            case KeyCode::DownArrow: return DIK_DOWN;
            case KeyCode::LeftArrow: return DIK_LEFT;
            case KeyCode::RightArrow: return DIK_RIGHT;
            case KeyCode::Space: return DIK_SPACE;
            }
            return 0;
        }

        void Init(win32::KeyboardDevice* pKeyboardDevice, win32::MouseDevice* pMouseDevice) {
            GetInputSystem().pKeyboardDevice = pKeyboardDevice;
            GetInputSystem().pMouseDevice = pMouseDevice;
        }
#endif // _WIN32

        bool GetKey(KeyCode keyCode) {
            if (!GetInputSystem().pKeyboardDevice) throw std::runtime_error("The input system is unavaliable.");
            return GetInputSystem().pKeyboardDevice->GetKey(Win32KeyCodeMapping(keyCode));
        }

        bool GetKeyDown(KeyCode keyCode) {
            if (!GetInputSystem().pKeyboardDevice) throw std::runtime_error("The input system is unavaliable.");
            return GetInputSystem().pKeyboardDevice->GetKeyDown(Win32KeyCodeMapping(keyCode));
        }

        bool GetKeyUp(KeyCode keyCode) {
            if (!GetInputSystem().pKeyboardDevice) throw std::runtime_error("The input system is unavaliable.");
            return GetInputSystem().pKeyboardDevice->GetKeyUp(Win32KeyCodeMapping(keyCode));
        }

    }
}