#include "win32_input.hpp"

#ifdef _WIN32

namespace engine {
	namespace win32 {

		bool KeyboardDevice::Update() {
			if (FAILED(device->Acquire())) {
				return false;
			}

			lastState = currentState;

			if (FAILED(device->GetDeviceState(sizeof(uint8_t) * 256, currentState.data()))) {
				return false;
			}

			return true;
		}

		bool KeyboardDevice::GetKey(uint8_t dikValue) {
			return currentState[dikValue] & 0x80;
		}

		bool KeyboardDevice::GetKeyDown(uint8_t dikValue) {
			return (currentState[dikValue] & 0x80) && !(lastState[dikValue] & 0x80);
		}

		bool KeyboardDevice::GetKeyUp(uint8_t dikValue) {
			return !(currentState[dikValue] & 0x80) && (lastState[dikValue] & 0x80);
		}

		bool MouseDevice::Update() {
			if (FAILED(device->Acquire())) {
				return false;
			}

			lastState = currentState;

			if (FAILED(device->GetDeviceState(sizeof(DIMOUSESTATE), static_cast<LPVOID>(&currentState)))) {
				return false;
			}

			return true;
		}

		bool MouseDevice::GetMouse(uint8_t buttonValue) {
			return currentState.rgbButtons[buttonValue] & 0x80;
		}

		bool MouseDevice::GetMouseDown(uint8_t buttonValue) {
			return (currentState.rgbButtons[buttonValue] & 0x80) && !(lastState.rgbButtons[buttonValue] & 0x80);
		}

		bool MouseDevice::GetMouseUp(uint8_t buttonValue) {
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

		KeyboardDevice DirectInputSystem::CreateKeyBoardInputSystem(HWND hWnd) {
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

			KeyboardDevice device(keyboardInputDevice);
			return device;
		}

		MouseDevice DirectInputSystem::CreateMouseInputSystem(HWND hWnd) {
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

			MouseDevice device(mouseInputDevice);
			return device;
		}

	}
}

#endif // _WIN32