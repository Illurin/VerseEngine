#include "engine.hpp"

namespace engine {

	Engine::Engine(const char* configPath) {
		GlobalConfig::configPath = configPath; 
	}

	void Engine::Start(const PlatformInfo& platformInfo) {
		auto instanceInfo = rhi::InstanceCreateInfo()
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
		
		auto colorAttachmentDescription = rhi::AttachmentDescription()
			.SetFormat(swapchainFormat)
			.SetSampleCount(rhi::SampleCount::Count1)
			.SetLoadOp(rhi::AttachmentLoadOp::Clear)
			.SetStoreOp(rhi::AttachmentStoreOp::Store)
			.SetInitialLayout(rhi::ImageLayout::Undefined)
			.SetPassLayout(rhi::ImageLayout::ColorAttachment)
			.SetFinalLayout(rhi::ImageLayout::Present);
		
		auto renderPassInfo = rhi::RenderPassCreateInfo()
			.SetPipelineType(rhi::PipelineType::Graphics)
			.SetColorAttachmentCount(1)
			.SetPColorAttachments(&colorAttachmentDescription);
		
		renderPass = device->CreateRenderPass(renderPassInfo);
		
		std::vector<char> shaderSource;
		auto shaderCompiler = instance->CreateShaderCompiler();
		
		shaderCompiler.Compile(StringToWString(Path("shader/hlsl/vertex_shader.hlsl").GetAbsolutePath()).c_str(), L"main", ShaderCompileArgument::vs_profile);
		shaderCompiler.GetSourceObject(shaderSource);
		
		auto shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Vertex);
		
		auto vertexShaderModule = device->CreateShaderModule(shaderModuleInfo);
		
		shaderCompiler.Compile(StringToWString(Path("shader/hlsl/fragment_shader.hlsl").GetAbsolutePath()).c_str(), L"main", ShaderCompileArgument::ps_profile);
		shaderCompiler.GetSourceObject(shaderSource);
		
		shaderModuleInfo = rhi::ShaderModuleCreateInfo()
			.SetSourceCode(shaderSource)
			.SetPEntryPoint("main")
			.SetShaderStage(rhi::ShaderStage::Fragment);
		
		auto fragmentShaderModule = device->CreateShaderModule(shaderModuleInfo);
		
		std::array<rhi::ShaderModule, 2> shaderModules = { vertexShaderModule, fragmentShaderModule };
		
		auto shaderStageInfo = rhi::PipelineShaderStageInfo()
			.SetShaderModuleCount(static_cast<uint32_t>(shaderModules.size())).SetPShaderModules(shaderModules.data());
		
		auto inputAssemblyInfo = rhi::PipelineInputAssemblyInfo()
			.SetTopology(rhi::PrimitiveTopology::TriangleList)
			.SetPrimitivieRestart(false);
		
		auto rasterizationInfo = rhi::PipelineRasterizationInfo()
			.SetLineWidth(1.0f)
			.SetSmoothLine(true)
			.SetPolygonMode(rhi::PolygonMode::Solid)
			.SetCullMode(rhi::CullMode::None)
			.SetDepthClamp(false)
			.SetDepthBias(0.0f)
			.SetSlopeScaledDepthBias(0.0f);
		
		auto scissor = rhi::Rect2D()
			.SetExtent(renderExtent)
			.SetOffset(rhi::Offset2D().SetX(0).SetY(0));
		
		auto viewport = rhi::Viewport()
			.SetWidth(static_cast<float>(renderExtent.width)).SetHeight(static_cast<float>(renderExtent.height))
			.SetX(0.0f).SetY(0.0f)
			.SetMinDepth(0.0f).SetMaxDepth(0.0f);
		
		auto viewportInfo = rhi::PipelineViewportInfo()
			.SetPScissors(&scissor).SetScissorCount(1)
			.SetPViewports(&viewport).SetViewportCount(1);
		
		auto depthStencilInfo = rhi::PipelineDepthStencilInfo()
			.SetDepthTestEnable(false)
			.SetDepthWriteEnable(false)
			.SetStencilTestEnable(false);
		
		auto colorAttachmentInfo = rhi::ColorBlendAttachmentInfo()
			.SetBlendEnable(false);
		
		auto colorBlendInfo = rhi::PipelineColorBlendInfo()
			.SetPAttachments(&colorAttachmentInfo).SetAttachmentCount(1);
		
		auto multisampleInfo = rhi::PipelineMultisampleInfo()
			.SetRasterizationSamples(rhi::SampleCount::Count1)
			.SetSampleShadingEnable(false)
			.SetAlphaToCoverageEnable(false);
		
		auto pipelineInfo = rhi::GraphicsPipelineCreateInfo()
			.SetShaderStageInfo(shaderStageInfo)
			.SetInputAssemblyInfo(inputAssemblyInfo)
			.SetRasterizationInfo(rasterizationInfo)
			.SetViewportInfo(viewportInfo)
			.SetDepthStencilInfo(depthStencilInfo)
			.SetColorBlendInfo(colorBlendInfo)
			.SetMultisampleInfo(multisampleInfo)
			.SetRenderPass(renderPass);
		
		pipeline = device->CreateGraphicsPipeline(pipelineInfo);
		
		vertexShaderModule->Destroy();
		fragmentShaderModule->Destroy();
		
		auto commandPoolInfo = rhi::CommandPoolCreateInfo()
			.SetQueue(queue);
		
		commandPool = device->CreateCommandPool(commandPoolInfo);
		
		commandBuffers = commandPool->AllocateCommandBuffers(frameCount);
		
		for (auto& swapchainImage : swapchainImages) {
			auto imageViewInfo = rhi::ImageViewInfo()
				.SetImage(swapchainImage)
				.SetFormat(swapchainFormat)
				.SetViewType(rhi::ImageViewType::Image2D)
				.SetBaseMipLevel(0)
				.SetMipLevelCount(1)
				.SetBaseArrayLayer(0)
				.SetArrayLayerCount(1);
		
			auto framebufferInfo = rhi::FramebufferCreateInfo()
				.SetWidth(renderExtent.width)
				.SetHeight(renderExtent.height)
				.SetLayers(1)
				.SetColorAttachmentCount(1)
				.SetPColorAttachments(&imageViewInfo)
				.SetRenderPass(renderPass);
		
			framebuffers.push_back(device->CreateFramebuffer(framebufferInfo));
		}

		fence = device->CreateFence(rhi::FenceCreateInfo());
	}

	void Engine::Run() {
		CalculateDeltaTime();
		CalculateFPS();
		Tick();
	}

	void Engine::Exit() {
		for (auto& framebuffer : framebuffers) framebuffer->Destroy();
		fence->Destroy();
		swapchain->Destroy();
		pipeline->Destroy();
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
		
		auto clearColorValue = rhi::ClearColorValue().SetColorValue(std::array<float, 4>({ 0.0f, 0.0f, 0.0f, 0.0f }));

		auto renderPassBeginInfo = rhi::RenderPassBeginInfo()
			.SetRenderPass(renderPass)
			.SetFramebuffer(framebuffers[swapchainImageIndex])
			.SetPClearColorValues(&clearColorValue);
		commandBuffer->BeginRenderPass(renderPassBeginInfo);

		// Draw Call
		commandBuffer->BindPipeline(pipeline);

		commandBuffer->Draw(3, 1, 0, 0);

		commandBuffer->EndRenderPass();
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