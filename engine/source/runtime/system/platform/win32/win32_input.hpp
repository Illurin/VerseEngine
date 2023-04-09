#pragma once

#ifdef _WIN32

#include "window_system.hpp"

#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

#include <array>

namespace engine {
	namespace win32 {
		
		class KeyboardDevice {
		public:
			KeyboardDevice(IDirectInputDevice8* device) : device(device) {}

			bool Update();

			bool GetKey(uint8_t dikValue);
			bool GetKeyDown(uint8_t dikValue);
			bool GetKeyUp(uint8_t dikValue);

		private:
			IDirectInputDevice8* device;

			std::array<uint8_t, 256> currentState{};
			std::array<uint8_t, 256> lastState{};
		};

		class MouseDevice {
		public:
			MouseDevice(IDirectInputDevice8* device) : device(device) {}

			bool Update();

			bool GetMouse(uint8_t buttonValue);
			bool GetMouseDown(uint8_t buttonValue);
			bool GetMouseUp(uint8_t buttonValue);

		private:
			IDirectInputDevice8* device;

			DIMOUSESTATE currentState{};
			DIMOUSESTATE lastState{};
		};

		class DirectInputSystem {
		public:
			explicit DirectInputSystem(HINSTANCE hInstance);
			DirectInputSystem(const DirectInputSystem&) = delete;
			~DirectInputSystem();

			KeyboardDevice CreateKeyBoardInputSystem(HWND hWnd);
			MouseDevice CreateMouseInputSystem(HWND hWnd);

		private:
			IDirectInput8* directInput{ nullptr };

			IDirectInputDevice8* keyboardInputDevice{ nullptr };
			IDirectInputDevice8* mouseInputDevice{ nullptr };
		};

	}
}

#endif // _WIN32