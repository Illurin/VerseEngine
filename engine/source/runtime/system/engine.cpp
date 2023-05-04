#include "engine.hpp"

#include "runtime/system/platform/platform_info.hpp"
#include "runtime/render/render_device.hpp"
#include "runtime/framework/scene/scene_manager.hpp"

namespace engine {

    Engine::Engine(const char* directory) {
        Path::SetDirectory(directory);
    }

    void Engine::Start(const PlatformInfo& platformInfo) {

        // Initialize render device

        auto instanceInfo = rhi::InstanceCreateInfo()
            .SetApplicationVersion(1)
            .SetPApplicationName("Test application")
            .SetPApiName("Vulkan");
        RenderDevice::Initialize(instanceInfo);

        auto swapchainInfo = rhi::SwapchainCreateInfo()
            .SetFrameCount(3)
            .SetAlphaMode(rhi::AlphaMode::Opaque)
            .SetImageUsage(rhi::ImageUsage::ColorAttachment)
            .SetPlatformInfo(platformInfo)
            .SetQueue(RenderDevice::GetQueue());
        RenderDevice::SetSwapchain(swapchainInfo);

        // Initialize scene manager

        SceneManager::Entry();
    }

    void Engine::Run() {
        CalculateDeltaTime();
        CalculateFps();
        Tick();
    }

    void Engine::Exit() {
        SceneManager::Clear();
        RenderDevice::Clear();
    }

    void Engine::Tick() {
        SceneManager::Update();
    }

    void Engine::CalculateDeltaTime() {
        auto currentTimePoint = std::chrono::steady_clock::now();
        std::chrono::duration<double> elapsed = currentTimePoint - timePoint;
        timePoint = currentTimePoint;
        Time::SetDeltaTime(elapsed.count());
    }

    void Engine::CalculateFps() {
        Time::SetFps(1.0 / Time::GetDeltaTime());
    }

}