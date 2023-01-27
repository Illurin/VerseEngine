#include "engine.hpp"

namespace engine {

	Engine::Engine(const char* configPath) {
		GlobalConfig::configPath = configPath; 
	}

	void Engine::Start(const PlatformInfo& platformInfo) {
		renderWidth = platformInfo.windowWidth;
		renderHeight = platformInfo.windowHeight;

		instance = new VkWrapperInstance();

		auto instanceInfo = rhi::InstanceInitInfo()
			.SetPApiName("Vulkan")
			.SetPApplicationName("Test application")
			.SetApplicationVersion(1);

		instance->Init(instanceInfo);

		auto queueInfo = rhi::QueueCreateInfo()
			.SetQueueType(rhi::QueueType::Graphics);

		auto deviceInfo = rhi::DeviceCreateInfo()
			.SetPhysicalDeviceId(0)
			.SetPQueueCreateInfos(&queueInfo)
			.SetQueueCreateInfoCount(1);

		device = instance->CreateDevice(deviceInfo);

		auto queue = device->GetQueue(0);

		auto swapchainInfo = rhi::SwapchainCreateInfo()
			.SetFormat(rhi::Format::R8G8B8A8Unorm)
			.SetFrameCount(frameCount)
			.SetAlphaMode(rhi::AlphaMode::Opaque)
			.SetImageExtent(rhi::Extent2D().SetWidth(renderWidth).SetHeight(renderHeight))
			.SetImageUsage(rhi::ImageUsage::ColorAttachment)
			.SetPlatformInfo(platformInfo)
			.SetQueue(queue);

		swapchain = device->CreateSwapchain(swapchainInfo);
		
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
			.SetPolygonMode(rhi::PolygonMode::Fill)
			.SetCullMode(rhi::CullMode::None)
			.SetDepthClamp(false)
			.SetDepthBias(0.0f)
			.SetSlopeScaledDepthBias(0.0f);

		auto scissor = rhi::Rect2D()
			.SetExtent(rhi::Extent2D().SetWidth(renderWidth).SetHeight(renderHeight))
			.SetOffset(rhi::Offset2D().SetX(0).SetY(0));

		auto viewport = rhi::Viewport()
			.SetWidth(static_cast<float>(renderWidth)).SetHeight(static_cast<float>(renderHeight))
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
			.SetMultisampleInfo(multisampleInfo);

		pipeline = device->CreateGraphicsPipeline(pipelineInfo);

		vertexShaderModule->Destroy();
		fragmentShaderModule->Destroy();

		auto commandPoolInfo = rhi::CommandPoolCreateInfo()
			.SetQueue(device->GetQueue(0));

		commandPool = device->CreateCommandPool(commandPoolInfo);

		commandBuffers = commandPool->AllocateCommandBuffers(frameCount);


	}

	void Engine::Run() {
		CalculateDeltaTime();
		CalculateFPS();
		Tick();
	}

	void Engine::Exit() {
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