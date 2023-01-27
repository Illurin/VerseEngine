#pragma once

#include "win32/window_system.hpp"

namespace engine {

	struct PlatformInfo {
		PlatformInfo& SetWindowWidth(uint32_t windowWidth) { this->windowWidth = windowWidth; return *this; }
		PlatformInfo& SetWindowHeight(uint32_t windowHeight) { this->windowHeight = windowHeight; return *this; }

		uint32_t windowWidth;
		uint32_t windowHeight;

#ifdef _WIN32
		PlatformInfo& SetHInstance(HINSTANCE hInstance) { this->hInstance = hInstance; return *this; }
		PlatformInfo& SetHWnd(HWND hWnd) { this->hWnd = hWnd; return *this; }

		HINSTANCE hInstance;
		HWND hWnd;
#endif // _WIN32
	};

}