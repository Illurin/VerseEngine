#include "system/engine.hpp"

using namespace engine;

#ifdef _WIN32

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
	Engine engine("C:/Programming/Github Repository/VerseEngine/engine");

	uint32_t initialWidth = 640;
	uint32_t initialHeight = 480;

	win32::WindowSystem windowSystem(hInstance, lpCmdLine, nCmdShow);
	auto windowCreateInfo = win32::WindowCreateInfo()
		.SetWidth(static_cast<int>(initialWidth))
		.SetHeight(static_cast<int>(initialHeight))
		.SetWindowName("Test window");
	windowSystem.InitWindow(windowCreateInfo);

	auto platformInfo = PlatformInfo()
		.SetWindowWidth(initialWidth)
		.SetWindowHeight(initialHeight)
		.SetHInstance(windowSystem.GetHInstance())
		.SetHWnd(windowSystem.GetMainHWnd());

	engine.Start(platformInfo);

	while (true) {
		engine.Run();

		if (!windowSystem.MessageLoop()) {
			engine.Exit();
			return 0;
		}
	}

	return 0;
}

#endif // _WIN32