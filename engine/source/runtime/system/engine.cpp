#include "engine.hpp"

namespace engine {

	Engine::Engine(const char* configPath) {
		GlobalConfig::configPath = configPath; 
	}

	void Engine::Start(const PlatformInfo& platformInfo) {
		auto instanceInfo = rhi::InstanceCreateInfo()
			//.SetPApiName("Direct3D12")
			.SetPApiName("Vulkan")
			.SetPApplicationName("Test application")
			.SetApplicationVersion(1);

		instance = rhi::CreateInstance(instanceInfo);

		auto queueInfo = rhi::QueueCreateInfo()
			.SetQueueType(rhi::QueueType::Graphics);
		
		auto deviceInfo = rhi::DeviceCreateInfo()
			.SetPhysicalDeviceId(1)
			.SetPQueueCreateInfos(&queueInfo)
			.SetQueueCreateInfoCount(1);
		
		device = instance->CreateDevice(deviceInfo);

		queue = device->GetQueue(0);

		auto swapchainInfo = rhi::SwapchainCreateInfo()
			.SetFormat(swapchainFormat)
			.SetFrameCount(frameCount)
			.SetAlphaMode(rhi::AlphaMode::Opaque)
			.SetImageUsage(rhi::ImageUsage::ColorAttachment)
			.SetPlatformInfo(platformInfo)
			.SetQueue(queue);

		swapchain = device->CreateSwapchain(swapchainInfo);

		renderExtent = swapchain->GetImageExtent();
		auto swapchainImages = swapchain->GetImages();

		std::array<DefaultVertexData, 3> verticesData;
		verticesData[0].worldPos = math::Vector3f(0.0f, 0.5f, 0.0f);
		verticesData[0].color = math::Vector3f(1.0f, 0.0f, 0.0f);
		verticesData[1].worldPos = math::Vector3f(-0.5f, -0.5f, 0.0f);
		verticesData[1].color = math::Vector3f(0.0f, 1.0f, 0.0f);
		verticesData[2].worldPos = math::Vector3f(0.5f, -0.5f, 0.0f);
		verticesData[2].color = math::Vector3f(0.0f, 0.0f, 1.0f);

		auto shaderCompiler = instance->CreateShaderCompiler();

		// render pass
		DefaultPassCreateInfo defaultPassInfo;
		defaultPassInfo.device = device;
		defaultPassInfo.colorFormat = swapchainFormat;
		defaultPassInfo.renderExtent = renderExtent;
		defaultPassInfo.colorAttachments = swapchainImages;
		defaultPassInfo.shaderCompiler = &shaderCompiler;
		defaultPassInfo.vertexCount = static_cast<uint32_t>(verticesData.size());
		defaultPassInfo.pVertexData = verticesData.data();

		defaultPass = std::make_unique<DefaultPass>(defaultPassInfo);

		auto commandPoolInfo = rhi::CommandPoolCreateInfo()
			.SetQueue(queue);
		commandPool = device->CreateCommandPool(commandPoolInfo);
		commandBuffers = commandPool->AllocateCommandBuffers(frameCount);

		fence = device->CreateFence(rhi::FenceCreateInfo());
	}

	void Engine::Run() {
		CalculateDeltaTime();
		CalculateFPS();
		Tick();
	}

	void Engine::Exit() {
		defaultPass.reset();
		fence->Destroy();
		swapchain->Destroy();
		commandPool->Destroy();
		device->Destroy();
		instance->Destroy();
	}

	void Engine::Tick() {
		LogicalTick();
		RendererTick();
	}

	void Engine::LogicalTick() {

	}

	void Engine::RendererTick() {
		fence->Reset();
		uint32_t swapchainImageIndex = swapchain->AcquireNextImage(fence);
		auto& commandBuffer = commandBuffers[swapchainImageIndex];
		fence->Wait();
		
		commandBuffer->Reset();
		commandBuffer->Begin(rhi::CommandBufferBeginInfo());
		
		// render pass
		defaultPass->Draw(commandBuffer, swapchainImageIndex);

		commandBuffer->End();
		
		fence->Reset();
		queue->SubmitCommandBuffers(1, &commandBuffer, fence);
		fence->Wait();
		queue->PresentSwapchain(swapchain, swapchainImageIndex);
	}

	void Engine::CalculateDeltaTime() {
		auto currentTimePoint = std::chrono::steady_clock::now();
		std::chrono::duration<double> elapsed = currentTimePoint - timePoint;
		deltaTime = elapsed.count();
	}

	void Engine::CalculateFPS() {
		fps = 1.0 / deltaTime;
	}

}