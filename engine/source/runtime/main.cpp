#include "runtime/system/engine.hpp"
#include "runtime/system/platform/input_system.hpp"

using namespace engine;

#ifdef _WIN32

int WINAPI wWinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPWSTR lpCmdLine, int nCmdShow) {
    Engine engine("C:/Programming/Github Repository/VerseEngine/engine");

    uint32_t initialWidth = 640;
    uint32_t initialHeight = 480;
    //uint32_t initialWidth = 1440;
    //uint32_t initialHeight = 960;

    // Initialize window system

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

    // Initialize input system

    win32::DirectInputSystem inputSystem(hInstance);
    auto keyboradDevice = inputSystem.CreateKeyBoardInputSystem(windowSystem.GetMainHWnd());
    auto mouseDevice = inputSystem.CreateMouseInputSystem(windowSystem.GetMainHWnd());
    input::Init(&keyboradDevice, &mouseDevice);

    // Start engine

    engine.Start(platformInfo);

    while (true) {
        keyboradDevice.Update();
        mouseDevice.Update();
        engine.Run();

        if (!windowSystem.MessageLoop()) {
            engine.Exit();
            return 0;
        }
    }

    return 0;
}

#endif // _WIN32