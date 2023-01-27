#include "window_system.hpp"

#ifdef _WIN32

namespace engine {
	namespace win32 {

		LRESULT CALLBACK WndProc(HWND hWnd, UINT message, WPARAM wParam, LPARAM lParam) {
			switch (message) {
			case WM_DESTROY: PostQuitMessage(0); break;
			default: return DefWindowProc(hWnd, message, wParam, lParam);
			}
			return 0;
		}

		void WindowSystem::InitWindow(const WindowCreateInfo& info) {
			WNDCLASSEX wcex = {};
			wcex.hIcon = LoadIcon(hInstance, IDI_QUESTION);
			wcex.cbSize = sizeof(WNDCLASSEX);
			wcex.style = CS_GLOBALCLASS;
			wcex.lpfnWndProc = WndProc;
			wcex.cbClsExtra = 0;
			wcex.cbWndExtra = 0;
			wcex.hInstance = hInstance;
			wcex.hCursor = LoadCursor(nullptr, IDC_ARROW);
			wcex.hbrBackground = (HBRUSH)GetStockObject(NULL_BRUSH);
			wcex.lpszClassName = L"MainWindowClass";
			RegisterClassEx(&wcex);

			mainHWnd = CreateWindow(L"MainWindowClass", StringToWString(info.windowName).c_str(), info.windowStyle, info.initialX, info.initialY, info.width, info.height, nullptr, nullptr, hInstance, nullptr);
		
			if (!mainHWnd) {
				throw std::runtime_error("Init win32 window failed");
			}

			ShowWindow(mainHWnd, nCmdShow);
			UpdateWindow(mainHWnd);
		}

		bool WindowSystem::MessageLoop() const {
			MSG msg = {};
			while (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE)) {
				if (msg.message != WM_QUIT) {
					TranslateMessage(&msg);
					DispatchMessage(&msg);
				}
				else {
					return false;
				}
			}
			return true;
		}

	}
}

#endif // _WIN32