#include "engine.hpp"

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
			.SetFrameCount(2)
			.SetAlphaMode(rhi::AlphaMode::Opaque)
			.SetImageUsage(rhi::ImageUsage::ColorAttachment)
			.SetPlatformInfo(platformInfo)
			.SetQueue(RenderDevice::GetQueue());
		RenderDevice::SetSwapchain(swapchainInfo);

		// Create main scene

		pScene = std::make_unique<MainScene>();
		pScene->Start();
	}

	void Engine::Run() {
		CalculateDeltaTime();
		CalculateFps();
		Tick();
	}

	void Engine::Exit() {
		pScene.reset();
		RenderDevice::Clear();
	}

	void Engine::Tick() {
		pScene->Update();
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