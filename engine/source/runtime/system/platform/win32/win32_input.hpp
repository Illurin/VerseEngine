#pragma once

#ifdef _WIN32

#include "window_system.hpp"

#define DIRECTINPUT_VERSION 0x800
#include <dinput.h>

namespace engine {
	namespace win32 {

		class KeyboardInputDevice {
		public:
			KeyboardInputDevice(IDirectInputDevice8* device) : device(device) {}

			void Update();
			
			bool GetKey(uint8_t dikValue);
			bool GetKeyDown(uint8_t dikValue);
			bool GetKeyUp(uint8_t dikValue);

		private:
			IDirectInputDevice8* device;

			std::array<uint8_t, 256> currentState{};
			std::array<uint8_t, 256> lastState{};
		};

		class MouseInputDevice {
		public:
			MouseInputDevice(IDirectInputDevice8* device) : device(device) {}

			void Update();

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
			DirectInputSystem(HINSTANCE hInstance);
			DirectInputSystem(const DirectInputSystem&) = delete;
			~DirectInputSystem();

			KeyboardInputDevice CreateKeyBoardInputSystem(HWND hWnd);
			MouseInputDevice CreateMouseInputSystem(HWND hWnd);

		private:
			IDirectInput8* directInput{ nullptr };

			IDirectInputDevice8* keyboardInputDevice{ nullptr };
			IDirectInputDevice8* mouseInputDevice{ nullptr };
		};

	}
}

#endif // _WIN32