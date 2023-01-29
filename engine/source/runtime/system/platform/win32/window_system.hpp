#pragma once

#include "runtime/system/file_system/path.hpp"

#ifdef _WIN32

#include <cstdint>
#include <stdexcept>

namespace engine {
	namespace win32 {

		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam);

		struct WindowCreateInfo {
			WindowCreateInfo& SetWidth(int width) { this->width = width; return *this; }
			WindowCreateInfo& SetHeight(int height) { this->height = height; return *this; }
			WindowCreateInfo& SetInitialX(int initialX) { this->initialX = initialX; return *this; }
			WindowCreateInfo& SetInitialY(int initialY) { this->initialY = initialY; return *this; }
			WindowCreateInfo& SetWindowName(const char* windowName) { this->windowName = windowName; return *this; }
			WindowCreateInfo& SetWindowStyle(DWORD windowStyle) { this->windowStyle = windowStyle; return *this; }

			int width;
			int height;
			int initialX{ CW_USEDEFAULT };
			int initialY{ CW_USEDEFAULT };
			const char* windowName;
			DWORD windowStyle{ WS_OVERLAPPED | WS_SYSMENU };
		};

		class WindowSystem {
		public:
			WindowSystem(HINSTANCE hInstance, LPWSTR lpCmdLine, int nCmdShow) 
				: hInstance(hInstance), nCmdShow(nCmdShow) {}
			~WindowSystem() {}

			void InitWindow(const WindowCreateInfo&);
			bool MessageLoop() const;

			HINSTANCE GetHInstance() const { return hInstance; }
			HWND GetMainHWnd() const { return mainHWnd; }

		private:
			HINSTANCE hInstance{ nullptr };
			HWND mainHWnd{ nullptr };

			int nCmdShow{ 0 };
		};

	}
}

#endif // _WIN32