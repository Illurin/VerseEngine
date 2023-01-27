#include "vulkan_rhi.hpp"

namespace engine {

	void VkWrapperInstance::Init(const rhi::InstanceInitInfo& info) {
#ifdef _DEBUG
		validationLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_LUNARG_monitor"
		};
		if (!VulkanUtil::CheckValidationLayerSupport(validationLayers)) {
			throw std::runtime_error("Do not support validation layers");
		}
		
		extensions.instance.push_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
#endif // _DEBUG

		extensions.instance.push_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.device.push_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifdef _WIN32
		extensions.instance.push_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // _WIN32

		CreateInstance(info);

#ifdef _DEBUG
		vk::DynamicLoader dl;
		PFN_vkGetInstanceProcAddr GetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

		auto debugMessengerInfo = vk::DebugUtilsMessengerCreateInfoEXT()
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose 
				| vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning 
				| vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral 
				| vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation 
				| vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(VulkanUtil::DebugCallback))
			.setPUserData(nullptr);

		vk::DispatchLoaderDynamic dispatch(instance, GetInstanceProcAddr);
		debugMessenger = instance.createDebugUtilsMessengerEXT(debugMessengerInfo, nullptr, dispatch);
		
		if (!debugMessenger) {
			throw std::runtime_error("Create debug messenger failed");
		}
#endif // _DEBUG

		this->info.apiName = info.pApiName;
		this->info.applicationName = info.pApplicationName;
		this->info.applicationVersion = info.applicationVersion;
	}

	void VkWrapperInstance::CreateInstance(const rhi::InstanceInitInfo& info) {
		auto applicationInfo = vk::ApplicationInfo()
			.setApiVersion(VK_API_VERSION_1_3)
			.setPEngineName("Verse Engine")
			.setEngineVersion(1)
			.setPApplicationName(info.pApplicationName)
			.setApplicationVersion(info.applicationVersion);

		auto instanceInfo = vk::InstanceCreateInfo()
			.setPEnabledExtensionNames(extensions.instance)
			.setPApplicationInfo(&applicationInfo)
			.setPEnabledLayerNames(validationLayers);

		instance = vk::createInstance(instanceInfo);
		
		if (!instance) {
			throw std::runtime_error("Create instance failed");
		}
	}

	std::vector<rhi::PhysicalDeviceInfo> VkWrapperInstance::GetPhysicalDeviceInfo() const {
		auto gpus = instance.enumeratePhysicalDevices();

		if (gpus.empty()) {
			throw std::runtime_error("Enumerate physical devices failed");
		}

		std::vector<rhi::PhysicalDeviceInfo> infos;

		for (uint32_t i = 0; i < gpus.size(); i++) {
			auto properties = gpus[i].getProperties();

			auto info = rhi::PhysicalDeviceInfo()
				.SetId(i)
				.SetVendorId(properties.vendorID)
				.SetDeviceId(properties.deviceID)
				.SetPDeviceDescription(properties.deviceName);

			infos.push_back(info);
		}
		return infos;
	}

	rhi::Device VkWrapperInstance::CreateDevice(const rhi::DeviceCreateInfo& info) const {
		auto gpus = instance.enumeratePhysicalDevices();

		if (info.physicalDeviceId >= gpus.size()) {
			throw std::runtime_error("Cannot find chosen physical device");
		}

		auto physicalDevice = gpus[info.physicalDeviceId];
		auto queueProp = physicalDevice.getQueueFamilyProperties();

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;

		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			uint32_t queueIndex;
			bool found = false;
			for (uint32_t j = 0; j < queueProp.size(); j++) {
				if (queueProp[j].queueFlags & VkEnumQueueType(info.pQueueCreateInfos[i].queueType).Get()) {
					queueIndex = j;
					found = true;
					break;
				}
			}

			if (!found) {
				throw std::runtime_error("Cannot find queue family");
			}
			
			std::array<float, 1> priorities = { 0.0f };
			auto deviceQueueInfo = vk::DeviceQueueCreateInfo()
				.setQueuePriorities(priorities)
				.setQueueFamilyIndex(queueIndex);
			deviceQueueInfos.push_back(deviceQueueInfo);
		}

		auto feature = vk::PhysicalDeviceFeatures()
			.setGeometryShader(VK_TRUE);

		auto feature13 = vk::PhysicalDeviceVulkan13Features()
			.setDynamicRendering(VK_TRUE);

		auto deviceExtensions = extensions.device;

		//deviceExtensions.push_back(VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME);
		//deviceExtensions.push_back(VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME);
		deviceExtensions.push_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);

		auto deviceInfo = vk::DeviceCreateInfo()
			.setPNext(&feature13)
			.setQueueCreateInfos(deviceQueueInfos)
			.setPEnabledExtensionNames(deviceExtensions)
			.setPEnabledFeatures(&feature);

		auto device = physicalDevice.createDevice(deviceInfo);

		if (!device) {
			throw std::runtime_error("Create logical device failed");
		}

		auto deviceWrapper = new VkWrapperDevice();
		deviceWrapper->instance = instance;
		deviceWrapper->device = device;
		deviceWrapper->physicalDevice = physicalDevice;

		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			auto queue = new VkWrapperQueue();
			queue->device = device;
			queue->queue = device.getQueue(deviceQueueInfos[i].queueFamilyIndex, 0);
			queue->queueFamilyIndex = deviceQueueInfos[i].queueFamilyIndex;
			deviceWrapper->queues.push_back(queue);
		}

		return deviceWrapper;
	}

	ShaderCompiler VkWrapperInstance::CreateShaderCompiler() const {
		ShaderCompiler shaderCompiler;
		shaderCompiler.AddArgument(ShaderCompileArgument::spirv);
		shaderCompiler.AddArgument(ShaderCompileArgument::vs_profile, ShaderCompileArgument::spirv_invert_y);
		shaderCompiler.AddArgument(ShaderCompileArgument::matrix_column_major);
		shaderCompiler.AddArgument(ShaderCompileArgument::spirv_debug);
		shaderCompiler.AddArgument(ShaderCompileArgument::treat_warnings_as_errors);
		shaderCompiler.AddIncludePath(StringToWString(GlobalConfig::configPath).c_str());
		return shaderCompiler;
	}

	

	rhi::Queue VkWrapperDevice::GetQueue(uint32_t queueIndex) const {
		if (queueIndex >= queues.size()) {
			throw std::runtime_error("Get queue out of range");
		}
		return queues[queueIndex];
	}

	rhi::Swapchain VkWrapperDevice::CreateSwapchain(const rhi::SwapchainCreateInfo& info) const {
#ifdef _WIN32
		auto surfaceInfo = vk::Win32SurfaceCreateInfoKHR()
			.setHwnd(info.platformInfo.hWnd)
			.setHinstance(info.platformInfo.hInstance);

		auto surface = instance.createWin32SurfaceKHR(surfaceInfo);
#endif // _WIN32

		if (!surface) {
			throw std::runtime_error("Create surface failed");
		}

		auto surfaceFormats = physicalDevice.getSurfaceFormatsKHR(surface);
		if (surfaceFormats.empty()) {
			throw std::runtime_error("Do not support any surface format");
		}

		auto surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		if (surfacePresentModes.empty()) {
			throw std::runtime_error("Do not support any surface present mode");
		}

		auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

		auto swapchainInfo = vk::SwapchainCreateInfoKHR()
			.setSurface(surface)
			.setImageFormat(VkEnumFormat(info.format).Get())
			.setMinImageCount(info.frameCount)
			.setImageExtent(surfaceCapabilities.currentExtent)
			.setPreTransform(surfaceCapabilities.currentTransform)
			.setPresentMode(vk::PresentModeKHR::eFifo)
			.setImageSharingMode(vk::SharingMode::eExclusive)
			.setCompositeAlpha(VkEnumAlphaMode(info.alphaMode).Get())
			.setImageColorSpace(surfaceFormats[0].colorSpace)
			.setImageUsage(VkEnumImageUsage(info.imageUsage).Get())
			.setImageArrayLayers(1)
			.setClipped(true);

		auto swapchain = device.createSwapchainKHR(swapchainInfo);
		
		if (!swapchain) {
			throw std::runtime_error("Create swapchain failed");
		}

		std::vector<rhi::Image> swapchainImages;
		auto images = device.getSwapchainImagesKHR(swapchain);
		for (auto& image : images) {
			auto imageWrapper = new VkWrapperImage();
			imageWrapper->image = image;
			swapchainImages.push_back(imageWrapper);
		}
		
		auto swapchainWrapper = new VkWrapperSwapchain();
		swapchainWrapper->device = device;
		swapchainWrapper->surface = surface;
		swapchainWrapper->swapchain = swapchain;
		swapchainWrapper->images = swapchainImages;
		return swapchainWrapper;
	}
	
	rhi::CommandPool VkWrapperDevice::CreateCommandPool(const rhi::CommandPoolCreateInfo& info) const {
		auto cmdPoolInfo = vk::CommandPoolCreateInfo()
			.setQueueFamilyIndex(static_cast<VkWrapperQueue*>(info.queue)->queueFamilyIndex)
			.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

		auto cmdPool = device.createCommandPool(cmdPoolInfo);
		
		if (!cmdPool) {
			throw std::runtime_error("Create command pool failed");
		}

		auto cmdPoolWrapper = new VkWrapperCommandPool();
		cmdPoolWrapper->device = device;
		cmdPoolWrapper->commandPool = cmdPool;
		return cmdPoolWrapper;
	}

	rhi::Buffer VkWrapperDevice::CreateBuffer(const rhi::BufferCreateInfo& info) const {
		auto bufferInfo = vk::BufferCreateInfo()
			.setSize(info.size)
			.setUsage(VkEnumBufferUsage(info.usage).Get());

		auto buffer = device.createBuffer(bufferInfo);

		if (!buffer) {
			throw std::runtime_error("Create buffer failed");
		}

		auto memReqs = device.getBufferMemoryRequirements(buffer);
		auto memoryInfo = vk::MemoryAllocateInfo()
			.setAllocationSize(memReqs.size);
		VulkanUtil::MemoryTypeFromProperties(physicalDevice.getMemoryProperties(), memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, memoryInfo.memoryTypeIndex);

		auto memory = device.allocateMemory(memoryInfo);

		if (!memory) {
			throw std::runtime_error("Allocate memory failed");
		}

		device.bindBufferMemory(buffer, memory, 0);

		auto bufferWrapper = new VkWrapperBuffer();
		bufferWrapper->device = device;
		bufferWrapper->buffer = buffer;
		bufferWrapper->memory = memory;
		return bufferWrapper;
	}

	/*rhi::BufferView VulkanInstance::CreateBufferView(rhi::Device& device, const rhi::BufferViewCreateInfo& info) const {
		auto bufferViewInfo = vk::BufferViewCreateInfo()
			.setBuffer(static_cast<const VkWrapperBuffer*>(info.buffer)->GetBuffer())
			.setFormat(VkEnumFormat(info.format).Get())
			.setRange(info.range)
			.setOffset(info.offset);

		auto bufferView = static_cast<VkWrapperDevice*>(device)->GetDevice().createBufferView(bufferViewInfo);

		if (!bufferView) {
			throw std::runtime_error("Create buffer view failed");
		}

		auto bufferViewWrapper = new VkWrapperBufferView();
		bufferViewWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetBufferView(bufferView);
		return bufferViewWrapper;
	}*/

	rhi::Image VkWrapperDevice::CreateImage(const rhi::ImageCreateInfo& info) const {
		auto imageInfo = vk::ImageCreateInfo()
			.setFormat(VkEnumFormat(info.format).Get())
			.setImageType(VkEnumImageType(info.imageType).Get())
			.setExtent(vk::Extent3D(info.extent.width, info.extent.height, info.extent.depth))
			.setUsage(VkEnumImageUsage(info.usage).Get())
			.setTiling(vk::ImageTiling::eOptimal)
			.setArrayLayers(info.arrayLayers)
			.setMipLevels(info.mipLevels)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setSamples(VkEnumSampleCount(info.sampleCount).Get());
		
		auto image = device.createImage(imageInfo);

		if (!image) {
			throw std::runtime_error("Create image failed");
		}

		auto memReqs = device.getImageMemoryRequirements(image);
		auto memoryInfo = vk::MemoryAllocateInfo()
			.setAllocationSize(memReqs.size);
		VulkanUtil::MemoryTypeFromProperties(physicalDevice.getMemoryProperties(), memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, memoryInfo.memoryTypeIndex);

		auto memory = device.allocateMemory(memoryInfo);

		if (!memory) {
			throw std::runtime_error("Allocate memory failed");
		}

		device.bindImageMemory(image, memory, 0);

		auto imageWrapper = new VkWrapperImage();
		imageWrapper->device = device;
		imageWrapper->image = image;
		imageWrapper->memory = memory;
		return imageWrapper;
	}

	/*rhi::ImageView VulkanInstance::CreateImageView(rhi::Device& device, const rhi::ImageViewCreateInfo& info) const {
		auto imageViewInfo = vk::ImageViewCreateInfo()
			.setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eR, vk::ComponentSwizzle::eG, vk::ComponentSwizzle::eB, vk::ComponentSwizzle::eA))
			.setImage(static_cast<VkWrapperImage*>(info.image)->GetImage())
			.setFormat(VkEnumFormat(info.format).Get())
			.setViewType(VkEnumImageViewType(info.imageViewType).Get())
			.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, info.baseMipLevel, info.mipLevelCount, 0, 1));

		auto imageView = static_cast<VkWrapperDevice*>(device)->GetDevice().createImageView(imageViewInfo);

		if (!imageView) {
			throw std::runtime_error("Create image view failed");
		}

		auto imageViewWrapper = new VkWrapperImageView();
		imageViewWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetImageView(imageView);
		return imageViewWrapper;
	}*/

	/*rhi::DescriptorPool VulkanInstance::CreateDescriptorPool(rhi::Device& device, const rhi::DescriptorPoolCreateInfo& info) const {
		auto poolSize = vk::DescriptorPoolSize()
			.setType(VkEnumDescriptorType(info.descriptorType).Get())
			.setDescriptorCount(info.descriptorCount);
		
		auto poolInfo = vk::DescriptorPoolCreateInfo()
			.setMaxSets(info.descriptorCount)
			.setPoolSizeCount(1)
			.setPPoolSizes(&poolSize);

		auto descriptorPool = static_cast<VkWrapperDevice*>(device)->GetDevice().createDescriptorPool(poolInfo);

		if (!descriptorPool) {
			throw std::runtime_error("Create descriptor pool failed");
		}

		auto descriptorPoolWrapper = new VkWrapperDescriptorPool();
		descriptorPoolWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetDescriptorPool(descriptorPool);
		return descriptorPoolWrapper;
	}*/

	rhi::ShaderModule VkWrapperDevice::CreateShaderModule(const rhi::ShaderModuleCreateInfo& info) const {
		auto shaderModuleInfo = vk::ShaderModuleCreateInfo()
			.setPCode(reinterpret_cast<const uint32_t*>(info.pSourceCode))
			.setCodeSize(static_cast<uint32_t>(info.sourceCodeSize));

		auto shaderModule = device.createShaderModule(shaderModuleInfo);

		auto shaderModuleWrapper = new VkWrapperShaderModule();
		shaderModuleWrapper->device = device;
		shaderModuleWrapper->entryPoint = info.pEntryPoint;
		shaderModuleWrapper->shaderStage = info.shaderStage;
		shaderModuleWrapper->shaderModule = shaderModule;
		return shaderModuleWrapper;
	}

	rhi::Pipeline VkWrapperDevice::CreateGraphicsPipeline(const rhi::GraphicsPipelineCreateInfo& info) const {
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos;
		for (uint32_t i = 0; i < info.shaderStageInfo.shaderModuleCount; i++) {
			auto shaderModule = static_cast<VkWrapperShaderModule*>(info.shaderStageInfo.pShaderModules[i]);
			shaderStageInfos.push_back(vk::PipelineShaderStageCreateInfo()
				.setModule(shaderModule->shaderModule)
				.setPName(shaderModule->entryPoint)
				.setStage(VkEnumShaderStage(shaderModule->shaderStage).Get()));
		}

		std::vector<vk::VertexInputBindingDescription> vertexBindingInfos;
		for (uint32_t i = 0; i < info.vertexInputInfo.bindingCount; i++) {
			auto& binding = info.vertexInputInfo.pBindings[i];
			vertexBindingInfos.push_back(vk::VertexInputBindingDescription()
				.setBinding(binding.bindingSlot)
				.setStride(binding.stride)
				.setInputRate(VkEnumVertexInputRate(binding.inputRate).Get()));
		}

		std::vector<vk::VertexInputAttributeDescription> vertexAttributeInfos;
		for (uint32_t i = 0; i < info.vertexInputInfo.attributeCount; i++) {
			auto& attribute = info.vertexInputInfo.pAttributes[i];
			vertexAttributeInfos.push_back(vk::VertexInputAttributeDescription()
				.setBinding(attribute.bindingSlot)
				.setLocation(attribute.location)
				.setFormat(VkEnumFormat(attribute.format).Get())
				.setOffset(attribute.offset));
		}
		
		auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo()
			.setVertexBindingDescriptions(vertexBindingInfos)
			.setVertexAttributeDescriptions(vertexAttributeInfos);

		auto inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo()
			.setTopology(VkEnumPrimitiveTopology(info.inputAssemblyInfo.topology).Get())
			.setPrimitiveRestartEnable(info.inputAssemblyInfo.primitivieRestart);

		auto tessellatonInfo = vk::PipelineTessellationStateCreateInfo()
			.setPatchControlPoints(info.tessellationInfo.patchControlPoints);
		
		auto rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
			.setCullMode(VkEnumCullMode(info.rasterizationInfo.cullMode).Get())
			.setPolygonMode(VkEnumPolygonMode(info.rasterizationInfo.polygonMode).Get())
			.setFrontFace(VkEnumFrontFace(info.rasterizationInfo.frontFace).Get())
			.setLineWidth(info.rasterizationInfo.lineWidth)
			.setDepthBiasEnable(VK_TRUE)
			.setDepthBiasConstantFactor(info.rasterizationInfo.depthBias)
			.setDepthBiasSlopeFactor(info.rasterizationInfo.slopeScaledDepthBias)
			.setDepthBiasClamp(info.rasterizationInfo.depthClamp)
			.setRasterizerDiscardEnable(VK_FALSE);

		std::vector<vk::Rect2D> scissors;
		for (uint32_t i = 0; i < info.viewportInfo.scissorCount; i++) {
			auto& scissorInfo = info.viewportInfo.pScissors[i];
			scissors.push_back(vk::Rect2D()
				.setExtent(vk::Extent2D(scissorInfo.extent.width, scissorInfo.extent.height))
				.setOffset(vk::Offset2D(scissorInfo.offset.x, scissorInfo.offset.y)));
		}

		std::vector<vk::Viewport> viewports;
		for (uint32_t i = 0; i < info.viewportInfo.viewportCount; i++) {
			auto& viewportInfo = info.viewportInfo.pViewports[i];
			viewports.push_back(vk::Viewport()
				.setWidth(viewportInfo.width).setHeight(viewportInfo.height)
				.setX(viewportInfo.x).setY(viewportInfo.y)
				.setMinDepth(viewportInfo.minDepth).setMaxDepth(viewportInfo.maxDepth));
		}

		auto viewportInfo = vk::PipelineViewportStateCreateInfo()
			.setScissors(scissors)
			.setViewports(viewports);

		auto frontOp = vk::StencilOpState()
			.setPassOp(VkEnumStencilOp(info.depthStencilInfo.frontOp.passOp).Get())
			.setFailOp(VkEnumStencilOp(info.depthStencilInfo.frontOp.failOp).Get())
			.setDepthFailOp(VkEnumStencilOp(info.depthStencilInfo.frontOp.depthFailOp).Get())
			.setCompareOp(VkEnumCompareOp(info.depthStencilInfo.frontOp.compareOp).Get())
			.setReference(1)
			.setCompareMask(static_cast<uint32_t>(info.depthStencilInfo.stencilCompareMask))
			.setCompareMask(static_cast<uint32_t>(info.depthStencilInfo.stencilWriteMask));

		auto backOp = vk::StencilOpState()
			.setPassOp(VkEnumStencilOp(info.depthStencilInfo.backOp.passOp).Get())
			.setFailOp(VkEnumStencilOp(info.depthStencilInfo.backOp.failOp).Get())
			.setDepthFailOp(VkEnumStencilOp(info.depthStencilInfo.backOp.depthFailOp).Get())
			.setCompareOp(VkEnumCompareOp(info.depthStencilInfo.backOp.compareOp).Get())
			.setReference(1)
			.setCompareMask(static_cast<uint32_t>(info.depthStencilInfo.stencilCompareMask))
			.setCompareMask(static_cast<uint32_t>(info.depthStencilInfo.stencilWriteMask));

		auto depthStencilInfo = vk::PipelineDepthStencilStateCreateInfo()
			.setDepthTestEnable(info.depthStencilInfo.depthTestEnable)
			.setDepthWriteEnable(info.depthStencilInfo.depthWriteEnable)
			.setDepthBoundsTestEnable(info.depthStencilInfo.depthBoundsTestEnable)
			.setMinDepthBounds(info.depthStencilInfo.minDepthBounds)
			.setMaxDepthBounds(info.depthStencilInfo.maxDepthBounds)
			.setDepthCompareOp(VkEnumCompareOp(info.depthStencilInfo.compareOp).Get())
			.setStencilTestEnable(info.depthStencilInfo.stencilTestEnable)
			.setFront(frontOp)
			.setBack(backOp);

		std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments;
		for (uint32_t i = 0; i < info.colorBlendInfo.attachmentCount; i++) {
			auto& attachmentInfo = info.colorBlendInfo.pAttachments[i];
			colorBlendAttachments.push_back(vk::PipelineColorBlendAttachmentState()
				.setBlendEnable(attachmentInfo.blendEnable)
				.setColorWriteMask(VkEnumColorComponent(attachmentInfo.colorWriteMask).Get())
				.setColorBlendOp(VkEnumBlendOp(attachmentInfo.colorBlendOp).Get())
				.setAlphaBlendOp(VkEnumBlendOp(attachmentInfo.alphaBlendOp).Get())
				.setSrcColorBlendFactor(VkEnumBlendFactor(attachmentInfo.srcColorBlendFactor).Get())
				.setDstColorBlendFactor(VkEnumBlendFactor(attachmentInfo.dstColorBlendFactor).Get())
				.setSrcAlphaBlendFactor(VkEnumBlendFactor(attachmentInfo.srcAlphaBlendFactor).Get())
				.setDstAlphaBlendFactor(VkEnumBlendFactor(attachmentInfo.dstAlphaBlendFactor).Get()));
		}

		auto colorBlendInfo = vk::PipelineColorBlendStateCreateInfo()
			.setAttachments(colorBlendAttachments)
			.setLogicOpEnable(VK_FALSE);

		auto multisampleInfo = vk::PipelineMultisampleStateCreateInfo()
			.setAlphaToOneEnable(VK_FALSE)
			.setAlphaToCoverageEnable(info.multisampleInfo.alphaToCoverageEnable)
			.setSampleShadingEnable(info.multisampleInfo.sampleShadingEnable)
			.setMinSampleShading(info.multisampleInfo.minSampleShading)
			.setRasterizationSamples(VkEnumSampleCount(info.multisampleInfo.rasterizationSamples).Get());

		auto pipelineLayout = device.createPipelineLayout(vk::PipelineLayoutCreateInfo());

		auto pipelineInfo = vk::GraphicsPipelineCreateInfo()
			.setStages(shaderStageInfos)
			.setPVertexInputState(&vertexInputInfo)
			.setPInputAssemblyState(&inputAssemblyInfo)
			.setPTessellationState(&tessellatonInfo)
			.setPRasterizationState(&rasterizationInfo)
			.setPViewportState(&viewportInfo)
			.setPDepthStencilState(&depthStencilInfo)
			.setPColorBlendState(&colorBlendInfo)
			.setPMultisampleState(&multisampleInfo)
			.setLayout(pipelineLayout);
		
		auto pipeline = device.createGraphicsPipeline(nullptr, pipelineInfo).value;
		
		if (!pipeline) {
			throw std::runtime_error("Create graphics pipeline failed");
		}

		device.destroy(pipelineLayout);
		
		auto pipelineWrapper = new VkWrapperPipeline();
		pipelineWrapper->device = device;
		pipelineWrapper->pipeline = pipeline;
		return pipelineWrapper;
	}

	rhi::Framebuffer VkWrapperDevice::CreateFramebuffer(const rhi::FramebufferCreateInfo& info) const {
		std::vector<vk::ImageView> colorAttachments;
		for (uint32_t i = 0; i < info.colorAttachmentCount; i++) {
			auto& colorAttachment = info.pColorAttachments[i];
			
			auto imageViewInfo = vk::ImageViewCreateInfo()
				.setImage(static_cast<VkWrapperImage*>(colorAttachment.image)->image)
				.setFormat(VkEnumFormat(colorAttachment.format).Get())
				.setViewType(VkEnumImageViewType(colorAttachment.viewType).Get())
				.setComponents(vk::ComponentMapping(
					VkEnumComponentSwizzle(colorAttachment.componentMapping.r).Get(),
					VkEnumComponentSwizzle(colorAttachment.componentMapping.g).Get(),
					VkEnumComponentSwizzle(colorAttachment.componentMapping.b).Get(),
					VkEnumComponentSwizzle(colorAttachment.componentMapping.a).Get()))
				.setSubresourceRange(vk::ImageSubresourceRange(
					vk::ImageAspectFlagBits::eColor,
					colorAttachment.baseMipLevel,
					colorAttachment.mipLevelCount,
					colorAttachment.baseArrayLayer,
					colorAttachment.arrayLayerCount));

			auto imageView = device.createImageView(imageViewInfo);

			if (!imageView) {
				throw std::runtime_error("Create image view failed");
			}

			colorAttachments.push_back(imageView);
		}

		auto imageViewInfo = vk::ImageViewCreateInfo()
			.setImage(static_cast<VkWrapperImage*>(info.depthAttachment.image)->image)
			.setFormat(VkEnumFormat(info.depthAttachment.format).Get())
			.setViewType(VkEnumImageViewType(info.depthAttachment.viewType).Get())
			.setComponents(vk::ComponentMapping(
				VkEnumComponentSwizzle(info.depthAttachment.componentMapping.r).Get(),
				VkEnumComponentSwizzle(info.depthAttachment.componentMapping.g).Get(),
				VkEnumComponentSwizzle(info.depthAttachment.componentMapping.b).Get(),
				VkEnumComponentSwizzle(info.depthAttachment.componentMapping.a).Get()))
			.setSubresourceRange(vk::ImageSubresourceRange(
				vk::ImageAspectFlagBits::eDepth,
				info.depthAttachment.baseMipLevel,
				info.depthAttachment.mipLevelCount,
				info.depthAttachment.baseArrayLayer,
				info.depthAttachment.arrayLayerCount));

		auto depthAttachment = device.createImageView(imageViewInfo);

		if (!depthAttachment) {
			throw std::runtime_error("Create image view failed");
		}

		imageViewInfo = vk::ImageViewCreateInfo()
			.setImage(static_cast<VkWrapperImage*>(info.stencilAttachment.image)->image)
			.setFormat(VkEnumFormat(info.stencilAttachment.format).Get())
			.setViewType(VkEnumImageViewType(info.stencilAttachment.viewType).Get())
			.setComponents(vk::ComponentMapping(
				VkEnumComponentSwizzle(info.stencilAttachment.componentMapping.r).Get(),
				VkEnumComponentSwizzle(info.stencilAttachment.componentMapping.g).Get(),
				VkEnumComponentSwizzle(info.stencilAttachment.componentMapping.b).Get(),
				VkEnumComponentSwizzle(info.stencilAttachment.componentMapping.a).Get()))
			.setSubresourceRange(vk::ImageSubresourceRange(
				vk::ImageAspectFlagBits::eDepth,
				info.stencilAttachment.baseMipLevel,
				info.stencilAttachment.mipLevelCount,
				info.stencilAttachment.baseArrayLayer,
				info.stencilAttachment.arrayLayerCount));

		auto stencilAttachment = device.createImageView(imageViewInfo);
		
		if (!stencilAttachment) {
			throw std::runtime_error("Create image view failed");
		}
		
		auto framebufferWrapper = new VkWrapperFramebuffer();
		framebufferWrapper->colorAttachments = colorAttachments;
		framebufferWrapper->depthAttachment = depthAttachment;
		framebufferWrapper->stencilAttachment = stencilAttachment;
		return framebufferWrapper;
	}

	rhi::Fence VkWrapperDevice::CreateFence(const rhi::FenceCreateInfo&) const {
		

		return new VkWrapperFence();
	}

	

	std::vector<rhi::Image> VkWrapperSwapchain::GetImages() const {
		return images;
	}

	uint32_t VkWrapperSwapchain::AcquireNextImage() {


		return 0;
	}



	void VkWrapperCommandPool::Reset() {

	}

	std::vector<rhi::CommandBuffer> VkWrapperCommandPool::AllocateCommandBuffers(uint32_t bufferCount) {
		auto cmdAllocInfo = vk::CommandBufferAllocateInfo()
			.setCommandPool(commandPool)
			.setLevel(vk::CommandBufferLevel::ePrimary)
			.setCommandBufferCount(bufferCount);

		auto cmdBuffer = device.allocateCommandBuffers(cmdAllocInfo);

		if (cmdBuffer.empty()) {
			throw std::runtime_error("Allocate command buffers failed");
		}

		std::vector<rhi::CommandBuffer> cmdWrappers;
		for (uint32_t i = 0; i < bufferCount; i++) {
			auto cmdWrapper = new VkWrapperCommandBuffer();
			cmdWrapper->commandPool = commandPool;
			cmdWrapper->commandBuffer = cmdBuffer[i];
			cmdWrappers.push_back(cmdWrapper);
		}

		commandBuffers.insert(commandBuffers.end(), cmdWrappers.begin(), cmdWrappers.end());
		
		return cmdWrappers;
	}



	void VkWrapperCommandBuffer::Reset() {

	}

	void VkWrapperCommandBuffer::Begin(const rhi::CommandBufferBeginInfo&) {

	}

	void VkWrapperCommandBuffer::End() {

	}

	void VkWrapperCommandBuffer::BeginRenderPass(const rhi::RenderPassBeginInfo&) {

	}

	void VkWrapperCommandBuffer::EndRenderPass() {

	}

	void VkWrapperCommandBuffer::BindPipeline(const rhi::Pipeline&) {

	}

	void VkWrapperCommandBuffer::BindVertexBuffer(uint32_t bindingCount, rhi::Buffer* pBuffer, uint64_t* pOffsets) {

	}

	void VkWrapperCommandBuffer::BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) {

	}

	void VkWrapperCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstCount, uint32_t firstInstance) const {

	}

	void VkWrapperCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstCount, int32_t vertexOffset, uint32_t firstInstance) const {

	}



	void VkWrapperFence::Reset() {

	}

	void VkWrapperFence::Wait() {

	}

	uint32_t VkWrapperFence::GetCurrentStatus() const {


		return 0;
	}



	void VkWrapperInstance::Destroy() {
#ifdef _DEBUG
		vk::DynamicLoader dl;
		PFN_vkGetInstanceProcAddr GetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");
		vk::DispatchLoaderDynamic dispatch(instance, GetInstanceProcAddr);

		instance.destroy(debugMessenger, nullptr, dispatch);
#endif // _DEBUG

		instance.destroy();
		
		delete this;
	}

	void VkWrapperDevice::Destroy() {
		device.destroy();
		for (auto& queue : queues) delete queue;
		delete this;
	}

	void VkWrapperSwapchain::Destroy() {
		device.destroy(swapchain);
		for (auto& image : images) delete image;
		delete this;
	}

	void VkWrapperCommandPool::Destroy() {
		device.destroy(commandPool);
		for (auto& cmdBuffer : commandBuffers) delete cmdBuffer;
		delete this;
	}

	void VkWrapperBuffer::Destroy() {
		device.destroy(buffer);
		device.free(memory);
		delete this;
	}

	void VkWrapperImage::Destroy() {
		device.destroy(image);
		device.free(memory);
		delete this;
	}

	/*void VulkanInstance::Destroy(rhi::ImageView& imageView) const {
		static_cast<VkWrapperImageView*>(imageView)->GetDevice().destroy(static_cast<VkWrapperImageView*>(imageView)->GetImageView());
		delete imageView;
	}

	void VulkanInstance::Destroy(rhi::DescriptorPool& descriptorPool) const {
		static_cast<VkWrapperDescriptorPool*>(descriptorPool)->GetDevice().destroy(static_cast<VkWrapperDescriptorPool*>(descriptorPool)->GetDescriptorPool());
		delete descriptorPool;
	}*/

	void VkWrapperShaderModule::Destroy() {
		device.destroy(shaderModule);
		delete this;
	}

	void VkWrapperPipeline::Destroy() {
		device.destroy(pipeline);
		delete this;
	}

	void VkWrapperFramebuffer::Destroy() {
		for (auto& colorAttachment : colorAttachments) device.destroy(colorAttachment);
		device.destroy(depthAttachment);
		device.destroy(stencilAttachment);
		delete this;
	}

	void VkWrapperFence::Destroy() {

	}

}