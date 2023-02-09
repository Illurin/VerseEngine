#include "win32_input.hpp"

#ifdef _WIN32

namespace engine {
	namespace win32 {

		void KeyboardInputDevice::Update() {
			if (FAILED(device->Acquire())) {
				throw std::runtime_error("Acquire keyboard input device failed");
			}

			lastState = currentState;

			if (FAILED(device->GetDeviceState(sizeof(uint8_t) * 256, currentState.data()))) {
				throw std::runtime_error("Get keyboard input device state failed");
			}
		}

		bool KeyboardInputDevice::GetKey(uint8_t dikValue) {
			return currentState[dikValue] & 0x80;
		}

		bool KeyboardInputDevice::GetKeyDown(uint8_t dikValue) {
			return (currentState[dikValue] & 0x80) && !(lastState[dikValue] & 0x80);
		}

		bool KeyboardInputDevice::GetKeyUp(uint8_t dikValue) {
			return !(currentState[dikValue] & 0x80) && (lastState[dikValue] & 0x80);
		}

		void MouseInputDevice::Update() {
			if (FAILED(device->Acquire())) {
				throw std::runtime_error("Acquire mouse input device failed");
			}

			lastState = currentState;

			if (FAILED(device->GetDeviceState(sizeof(DIMOUSESTATE), static_cast<LPVOID>(&currentState)))) {
				throw std::runtime_error("Get mouse input device state failed");
			}
		}

		bool MouseInputDevice::GetMouse(uint8_t buttonValue) {
			return currentState.rgbButtons[buttonValue] & 0x80;
		}

		bool MouseInputDevice::GetMouseDown(uint8_t buttonValue) {
			return (currentState.rgbButtons[buttonValue] & 0x80) && !(lastState.rgbButtons[buttonValue] & 0x80);
		}

		bool MouseInputDevice::GetMouseUp(uint8_t buttonValue) {
			return !(currentState.rgbButtons[buttonValue] & 0x80) && (lastState.rgbButtons[buttonValue] & 0x80);
		}

		DirectInputSystem::DirectInputSystem(HINSTANCE hInstance) {
			if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION, IID_IDirectInput8, reinterpret_cast<VOID**>(&directInput), NULL))) {
				throw std::runtime_error("Create direct input failed");
			}
		}

		DirectInputSystem::~DirectInputSystem() {
			directInput->Release();

			if (keyboardInputDevice) keyboardInputDevice->Release();
			if (mouseInputDevice) mouseInputDevice->Release();
		}

		KeyboardInputDevice DirectInputSystem::CreateKeyBoardInputSystem(HWND hWnd) {
			if (keyboardInputDevice) keyboardInputDevice->Release();

			if (FAILED(directInput->CreateDevice(GUID_SysKeyboard, &keyboardInputDevice, NULL))) {
				throw std::runtime_error("Create keyboard input device failed");
			}

			if (FAILED(keyboardInputDevice->SetDataFormat(&c_dfDIKeyboard))) {
				throw std::runtime_error("Set keyboard input device data format failed");
			}

			if (FAILED(keyboardInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
				throw std::runtime_error("Set keyboard input device cooperative level failed");
			}

			KeyboardInputDevice device(keyboardInputDevice);
			return device;
		}

		MouseInputDevice DirectInputSystem::CreateMouseInputSystem(HWND hWnd) {
			if (mouseInputDevice) mouseInputDevice->Release();

			if (FAILED(directInput->CreateDevice(GUID_SysMouse, &mouseInputDevice, NULL))) {
				throw std::runtime_error("Create mouse input device failed");
			}

			if (FAILED(mouseInputDevice->SetDataFormat(&c_dfDIMouse))) {
				throw std::runtime_error("Set mouse input device data format failed");
			}

			if (FAILED(mouseInputDevice->SetCooperativeLevel(hWnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE))) {
				throw std::runtime_error("Set mouse input device cooperative level failed");
			}

			MouseInputDevice device(mouseInputDevice);
			return device;
		}

	}
}

#endif // _WIN32