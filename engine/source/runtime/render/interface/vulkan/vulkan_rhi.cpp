#include "vulkan_rhi.hpp"

#ifdef _VULKAN

namespace engine {


	/* -------------------- VkWrapperInstance -------------------- */


	void VkWrapperInstance::Init(const rhi::InstanceCreateInfo& info) {
#ifdef _DEBUG
		validationLayers = {
			"VK_LAYER_KHRONOS_validation",
			"VK_LAYER_LUNARG_monitor"
		};
		if (!VulkanUtil::CheckValidationLayerSupport(validationLayers)) {
			throw std::runtime_error("Do not support validation layers");
		}
		
		extensions.instance.emplace_back(VK_KHR_GET_PHYSICAL_DEVICE_PROPERTIES_2_EXTENSION_NAME);
#endif // _DEBUG

		extensions.instance.emplace_back(VK_EXT_DEBUG_UTILS_EXTENSION_NAME);
		extensions.instance.emplace_back(VK_KHR_SURFACE_EXTENSION_NAME);
		extensions.device.emplace_back(VK_KHR_SWAPCHAIN_EXTENSION_NAME);

#ifdef _WIN32
		extensions.instance.emplace_back(VK_KHR_WIN32_SURFACE_EXTENSION_NAME);
#endif // _WIN32

		CreateInstance(info);

#ifdef _DEBUG
		vk::DynamicLoader dl;
		PFN_vkGetInstanceProcAddr GetInstanceProcAddr = dl.getProcAddress<PFN_vkGetInstanceProcAddr>("vkGetInstanceProcAddr");

		auto debugMessengerInfo = vk::DebugUtilsMessengerCreateInfoEXT()
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose |
				                vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning |
				                vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral |
				            vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation |
				            vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
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

	void VkWrapperInstance::CreateInstance(const rhi::InstanceCreateInfo& info) {
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

		std::vector<rhi::PhysicalDeviceInfo> infos(gpus.size());

		for (uint32_t i = 0; i < gpus.size(); i++) {
			auto properties = gpus[i].getProperties();

			auto info = rhi::PhysicalDeviceInfo()
				.SetId(i)
				.SetVendorId(properties.vendorID)
				.SetDeviceId(properties.deviceID)
				.SetDeviceDescription(properties.deviceName);

			infos[i] = info;
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

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos(info.queueCreateInfoCount);

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
			deviceQueueInfos[i] = deviceQueueInfo;
		}

		auto features = vk::PhysicalDeviceFeatures()
			.setGeometryShader(VK_TRUE)
			.setFillModeNonSolid(VK_TRUE);

		auto deviceExtensions = extensions.device;

		//deviceExtensions.emplace_back(VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME);
		//deviceExtensions.emplace_back(VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME);
		deviceExtensions.emplace_back(VK_KHR_SHADER_NON_SEMANTIC_INFO_EXTENSION_NAME);
		deviceExtensions.emplace_back(VK_EXT_LINE_RASTERIZATION_EXTENSION_NAME);

		auto lineRasterizationFeaturesEXT = vk::PhysicalDeviceLineRasterizationFeaturesEXT()
			.setSmoothLines(VK_TRUE);

		auto deviceInfo = vk::DeviceCreateInfo()
			.setPNext(&lineRasterizationFeaturesEXT)
			.setQueueCreateInfos(deviceQueueInfos)
			.setPEnabledExtensionNames(deviceExtensions)
			.setPEnabledFeatures(&features);

		auto device = physicalDevice.createDevice(deviceInfo);

		if (!device) {
			throw std::runtime_error("Create logical device failed");
		}
		
		auto deviceWrapper = new VkWrapperDevice();
		deviceWrapper->instance = instance;
		deviceWrapper->device = device;
		deviceWrapper->physicalDevice = physicalDevice;

		deviceWrapper->queues.resize(info.queueCreateInfoCount);
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			auto queue = new VkWrapperQueue();
			queue->device = device;
			queue->queue = device.getQueue(deviceQueueInfos[i].queueFamilyIndex, 0);
			queue->queueFamilyIndex = deviceQueueInfos[i].queueFamilyIndex;
			deviceWrapper->queues[i] = queue;
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


	/* -------------------- VkWrapperDevice -------------------- */


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
			.setImageExtent(!info.imageExtent.width && !info.imageExtent.height ?
				surfaceCapabilities.currentExtent :
				vk::Extent2D(info.imageExtent.width, info.imageExtent.height))
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
			swapchainImages.emplace_back(imageWrapper);
		}
		
		auto swapchainWrapper = new VkWrapperSwapchain();
		swapchainWrapper->instance = instance;
		swapchainWrapper->device = device;
		swapchainWrapper->surface = surface;
		swapchainWrapper->swapchain = swapchain;
		swapchainWrapper->images = swapchainImages;
		swapchainWrapper->imageExtent = rhi::Extent2D().SetWidth(swapchainInfo.imageExtent.width).SetHeight(swapchainInfo.imageExtent.height);
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

	rhi::Image VkWrapperDevice::CreateImage(const rhi::ImageCreateInfo& info) const {
		auto imageInfo = vk::ImageCreateInfo()
			.setFormat(VkEnumFormat(info.format).Get())
			.setImageType(VkEnumImageType(info.imageType).Get())
			.setExtent(vk::Extent3D(info.extent.width, info.extent.height, info.extent.depth))
			.setUsage(VkEnumImageUsage(info.usage).Get())
			.setInitialLayout(VkEnumImageLayout(info.initialLayout).Get())
			.setTiling(vk::ImageTiling::eOptimal)
			.setArrayLayers(info.arrayLayers)
			.setMipLevels(info.mipLevels)
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

	rhi::Sampler VkWrapperDevice::CreateSampler(const rhi::SamplerCreateInfo& info) const {
		auto samplerInfo = vk::SamplerCreateInfo()
			.setMinFilter(VkEnumFilter(info.minFilter).Get())
			.setMagFilter(VkEnumFilter(info.magFilter).Get())
			.setMipmapMode(VkEnumSamplerMipmapMode(info.mipFilter).Get())
			.setAddressModeU(VkEnumSamplerAddressMode(info.addressModeU).Get())
			.setAddressModeV(VkEnumSamplerAddressMode(info.addressModeV).Get())
			.setAddressModeW(VkEnumSamplerAddressMode(info.addressModeW).Get())
			.setMipLodBias(info.mipLodBias)
			.setMinLod(info.minLod)
			.setMaxLod(info.maxLod)
			.setAnisotropyEnable(info.anisotropyEnable)
			.setMaxAnisotropy(info.maxAnisotropy)
			.setCompareEnable(info.compareEnable)
			.setCompareOp(VkEnumCompareOp(info.compareOp).Get())
			.setBorderColor(VkEnumBorderColor(info.borderColor).Get());
		
		auto sampler = device.createSampler(samplerInfo);
		
		if (!sampler) {
			throw std::runtime_error("Create sampler failed");
		}

		auto samplerWrapper = new VkWrapperSampler();
		samplerWrapper->device = device;
		samplerWrapper->sampler = sampler;
		return samplerWrapper;
	}

	rhi::DescriptorPool VkWrapperDevice::CreateDescriptorPool(const rhi::DescriptorPoolCreateInfo& info) const {
		std::vector<vk::DescriptorPoolSize> poolSizes(info.poolSizeCount);
		for (uint32_t i = 0; i < info.poolSizeCount; i++) {
			poolSizes[i].setType(VkEnumDescriptorType(info.pPoolSizes[i].type).Get());
			poolSizes[i].setDescriptorCount(info.pPoolSizes[i].size);
		}

		auto descriptorPoolInfo = vk::DescriptorPoolCreateInfo()
			.setMaxSets(info.maxSetCount)
			.setPoolSizes(poolSizes)
			.setFlags(vk::DescriptorPoolCreateFlagBits::eFreeDescriptorSet);

		auto descriptorPool = device.createDescriptorPool(descriptorPoolInfo);

		if (!descriptorPool) {
			throw std::runtime_error("Create descriptor pool failed");
		}

		auto descriptorPoolWrapper = new VkWrapperDescriptorPool();
		descriptorPoolWrapper->device = device;
		descriptorPoolWrapper->descriptorPool = descriptorPool;
		descriptorPoolWrapper->descriptorSets.resize(info.maxSetCount);
		return descriptorPoolWrapper;
	}

	rhi::DescriptorSetLayout VkWrapperDevice::CreateDescriptorSetLayout(const rhi::DescriptorSetLayoutCreateInfo& info) const {
		std::vector<vk::DescriptorSetLayoutBinding> bindings(info.bindingCount);
		for (uint32_t i = 0; i < info.bindingCount; i++) {
			auto& binding = info.pBindings[i];
			bindings[i] = vk::DescriptorSetLayoutBinding()
				.setBinding(binding.binding)
				.setDescriptorCount(1)
				.setDescriptorType(VkEnumDescriptorType(binding.descriptorType).Get())
				.setStageFlags(VkEnumShaderStageFlags(binding.shaderStage).Get());

			if (binding.pStaticSampler) {
				vk::Sampler sampler = static_cast<VkWrapperSampler*>(*binding.pStaticSampler)->sampler;
				bindings[i].setPImmutableSamplers(&sampler);
			}
		}

		auto descriptorSetLayoutInfo = vk::DescriptorSetLayoutCreateInfo()
			.setBindings(bindings);

		auto descriptorSetLayout = device.createDescriptorSetLayout(descriptorSetLayoutInfo);

		if (!descriptorSetLayout) {
			throw std::runtime_error("Create descriptor set layout failed");
		}
		
		auto descriptorSetLayoutWrapper = new VkWrapperDescriptorSetLayout();
		descriptorSetLayoutWrapper->device = device;
		descriptorSetLayoutWrapper->descriptorSetLayout = descriptorSetLayout;
		return descriptorSetLayoutWrapper;
	}

	rhi::PipelineLayout VkWrapperDevice::CreatePipelineLayout(const rhi::PipelineLayoutCreateInfo& info) const {
		std::vector<vk::DescriptorSetLayout> setLayouts(info.descriptorSetLayoutCount);
		for (uint32_t i = 0; i < info.descriptorSetLayoutCount; i++) {
			auto descriptorSetLayout = static_cast<VkWrapperDescriptorSetLayout*>(info.pDescriptorSetLayouts[i]);
			setLayouts[i] = descriptorSetLayout->descriptorSetLayout;
		}

		auto pipelineLayoutInfo = vk::PipelineLayoutCreateInfo()
			.setSetLayouts(setLayouts);

		auto pipelineLayout = device.createPipelineLayout(pipelineLayoutInfo);

		if (!pipelineLayout) {
			throw std::runtime_error("Create pipeline layout failed");
		}

		auto pipelineLayoutWrapper = new VkWrapperPipelineLayout();
		pipelineLayoutWrapper->device = device;
		pipelineLayoutWrapper->pipelineLayout = pipelineLayout;
		return pipelineLayoutWrapper;
	}

	rhi::RenderPass VkWrapperDevice::CreateRenderPass(const rhi::RenderPassCreateInfo& info) const {
		auto subpassInfo = vk::SubpassDescription()
			.setPipelineBindPoint(VkEnumPipelineBindPoint(info.pipelineType).Get());
		
		uint32_t referenceIndex = 0;
		std::vector<vk::AttachmentReference> colorAttachmentReferences(info.colorAttachmentCount);
		for (uint32_t i = 0; i < info.colorAttachmentCount; i++) {
			colorAttachmentReferences[i] = vk::AttachmentReference()
				.setAttachment(referenceIndex)
				.setLayout(VkEnumImageLayout(info.pColorAttachments[i].passLayout).Get());
			referenceIndex++;
		}
		subpassInfo.setColorAttachments(colorAttachmentReferences);
		vk::AttachmentReference depthStencilAttachmentReference;
		if (info.pDepthStencilAttachment) {
			depthStencilAttachmentReference = vk::AttachmentReference()
				.setAttachment(referenceIndex)
				.setLayout(VkEnumImageLayout(info.pDepthStencilAttachment->passLayout).Get());
			subpassInfo.setPDepthStencilAttachment(&depthStencilAttachmentReference);
			referenceIndex++;
		}

		std::vector<vk::AttachmentDescription> attachmentDescriptions(info.colorAttachmentCount);
		for (uint32_t i = 0; i < info.colorAttachmentCount; i++) {
			auto& attachment = info.pColorAttachments[i];
			attachmentDescriptions[i] = vk::AttachmentDescription()
				.setFormat(VkEnumFormat(attachment.format).Get())
				.setSamples(VkEnumSampleCount(attachment.sampleCount).Get())
				.setInitialLayout(VkEnumImageLayout(info.pColorAttachments[i].initialLayout).Get())
				.setFinalLayout(VkEnumImageLayout(info.pColorAttachments[i].finalLayout).Get())
				.setLoadOp(VkEnumAttachmentLoadOp(attachment.loadOp).Get())
				.setStoreOp(VkEnumAttachmentStoreOp(attachment.storeOp).Get());
		}
		if (info.pDepthStencilAttachment) {
			attachmentDescriptions.emplace_back(vk::AttachmentDescription()
				.setFormat(VkEnumFormat(info.pDepthStencilAttachment->format).Get())
				.setSamples(VkEnumSampleCount(info.pDepthStencilAttachment->sampleCount).Get())
				.setInitialLayout(VkEnumImageLayout(info.pDepthStencilAttachment->initialLayout).Get())
				.setFinalLayout(VkEnumImageLayout(info.pDepthStencilAttachment->finalLayout).Get())
				.setLoadOp(VkEnumAttachmentLoadOp(info.pDepthStencilAttachment->loadOp).Get())
				.setStoreOp(VkEnumAttachmentStoreOp(info.pDepthStencilAttachment->storeOp).Get())
				.setStencilLoadOp(VkEnumAttachmentLoadOp(info.pDepthStencilAttachment->stencilLoadOp).Get())
				.setStencilStoreOp(VkEnumAttachmentStoreOp(info.pDepthStencilAttachment->stencilStoreOp).Get()));
		}
		
		auto renderPassInfo = vk::RenderPassCreateInfo()
			.setAttachments(attachmentDescriptions)
			.setSubpassCount(1)
			.setPSubpasses(&subpassInfo);

		auto renderPass = device.createRenderPass(renderPassInfo);

		if (!renderPass) {
			throw std::runtime_error("Create render pass failed");
		}

		auto renderPassWrapper = new VkWrapperRenderPass();
		renderPassWrapper->device = device;
		renderPassWrapper->renderPass = renderPass;
		return renderPassWrapper;
	}

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
		std::vector<vk::PipelineShaderStageCreateInfo> shaderStageInfos(info.shaderStageInfo.shaderModuleCount);
		for (uint32_t i = 0; i < info.shaderStageInfo.shaderModuleCount; i++) {
			auto shaderModule = static_cast<VkWrapperShaderModule*>(info.shaderStageInfo.pShaderModules[i]);
			shaderStageInfos[i] = vk::PipelineShaderStageCreateInfo()
				.setModule(shaderModule->shaderModule)
				.setPName(shaderModule->entryPoint)
				.setStage(VkEnumShaderStage(shaderModule->shaderStage).Get());
		}

		std::vector<vk::VertexInputBindingDescription> vertexBindingInfos(info.vertexInputInfo.bindingCount);
		for (uint32_t i = 0; i < info.vertexInputInfo.bindingCount; i++) {
			auto& binding = info.vertexInputInfo.pBindings[i];
			vertexBindingInfos[i] = vk::VertexInputBindingDescription()
				.setBinding(i)
				.setStride(binding.stride)
				.setInputRate(VkEnumVertexInputRate(binding.inputRate).Get());
		}

		std::vector<vk::VertexInputAttributeDescription> vertexAttributeInfos(info.vertexInputInfo.attributeCount);
		for (uint32_t i = 0; i < info.vertexInputInfo.attributeCount; i++) {
			auto& attribute = info.vertexInputInfo.pAttributes[i];
			vertexAttributeInfos[i] = vk::VertexInputAttributeDescription()
				.setBinding(attribute.binding)
				.setLocation(i)
				.setFormat(VkEnumFormat(attribute.format).Get())
				.setOffset(attribute.offset);
		}
		
		auto vertexInputInfo = vk::PipelineVertexInputStateCreateInfo()
			.setVertexBindingDescriptions(vertexBindingInfos)
			.setVertexAttributeDescriptions(vertexAttributeInfos);

		auto inputAssemblyInfo = vk::PipelineInputAssemblyStateCreateInfo()
			.setTopology(VkEnumPrimitiveTopology(info.inputAssemblyInfo.topology).Get())
			.setPrimitiveRestartEnable(info.inputAssemblyInfo.primitivieRestart);

		auto tessellatonInfo = vk::PipelineTessellationStateCreateInfo()
			.setPatchControlPoints(info.tessellationInfo.patchControlPoints);
		
		auto rasterizationLineInfoEXT = vk::PipelineRasterizationLineStateCreateInfoEXT()
			.setLineRasterizationMode(info.rasterizationInfo.smoothLine ?
				vk::LineRasterizationModeEXT::eRectangularSmooth :
				vk::LineRasterizationModeEXT::eDefault)
			.setStippledLineEnable(VK_FALSE);

		auto rasterizationInfo = vk::PipelineRasterizationStateCreateInfo()
			.setPNext(&rasterizationLineInfoEXT)
			.setCullMode(VkEnumCullMode(info.rasterizationInfo.cullMode).Get())
			.setPolygonMode(VkEnumPolygonMode(info.rasterizationInfo.polygonMode).Get())
			.setFrontFace(VkEnumFrontFace(info.rasterizationInfo.frontFace).Get())
			.setLineWidth(1.0f)
			.setDepthBiasEnable(VK_TRUE)
			.setDepthBiasConstantFactor(info.rasterizationInfo.depthBias)
			.setDepthBiasSlopeFactor(info.rasterizationInfo.slopeScaledDepthBias)
			.setDepthBiasClamp(info.rasterizationInfo.depthClamp)
			.setRasterizerDiscardEnable(VK_FALSE);

		std::vector<vk::Rect2D> scissors(info.viewportInfo.scissorCount);
		for (uint32_t i = 0; i < info.viewportInfo.scissorCount; i++) {
			auto& scissorInfo = info.viewportInfo.pScissors[i];
			scissors[i] = vk::Rect2D()
				.setExtent(vk::Extent2D(scissorInfo.extent.width, scissorInfo.extent.height))
				.setOffset(vk::Offset2D(scissorInfo.offset.x, scissorInfo.offset.y));
		}

		std::vector<vk::Viewport> viewports(info.viewportInfo.viewportCount);
		for (uint32_t i = 0; i < info.viewportInfo.viewportCount; i++) {
			auto& viewportInfo = info.viewportInfo.pViewports[i];
			viewports[i] = vk::Viewport()
				.setWidth(viewportInfo.width).setHeight(viewportInfo.height)
				.setX(viewportInfo.x).setY(viewportInfo.y)
				.setMinDepth(viewportInfo.minDepth).setMaxDepth(viewportInfo.maxDepth);
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
			.setDepthCompareOp(VkEnumCompareOp(info.depthStencilInfo.depthCompareOp).Get())
			.setStencilTestEnable(info.depthStencilInfo.stencilTestEnable)
			.setFront(frontOp)
			.setBack(backOp);

		std::vector<vk::PipelineColorBlendAttachmentState> colorBlendAttachments(info.colorBlendInfo.attachmentCount);
		for (uint32_t i = 0; i < info.colorBlendInfo.attachmentCount; i++) {
			auto& attachmentInfo = info.colorBlendInfo.pAttachments[i];
			colorBlendAttachments[i] = vk::PipelineColorBlendAttachmentState()
				.setBlendEnable(attachmentInfo.blendEnable)
				.setColorWriteMask(VkEnumColorComponent(attachmentInfo.colorWriteMask).Get())
				.setColorBlendOp(VkEnumBlendOp(attachmentInfo.colorBlendOp).Get())
				.setAlphaBlendOp(VkEnumBlendOp(attachmentInfo.alphaBlendOp).Get())
				.setSrcColorBlendFactor(VkEnumBlendFactor(attachmentInfo.srcColorBlendFactor).Get())
				.setDstColorBlendFactor(VkEnumBlendFactor(attachmentInfo.dstColorBlendFactor).Get())
				.setSrcAlphaBlendFactor(VkEnumBlendFactor(attachmentInfo.srcAlphaBlendFactor).Get())
				.setDstAlphaBlendFactor(VkEnumBlendFactor(attachmentInfo.dstAlphaBlendFactor).Get());
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
			.setLayout(static_cast<VkWrapperPipelineLayout*>(info.pipelineLayout)->pipelineLayout)
			.setRenderPass(static_cast<VkWrapperRenderPass*>(info.renderPass)->renderPass);
		
		auto pipeline = device.createGraphicsPipeline(nullptr, pipelineInfo).value;
		
		if (!pipeline) {
			throw std::runtime_error("Create graphics pipeline failed");
		}

		auto pipelineWrapper = new VkWrapperPipeline();
		pipelineWrapper->device = device;
		pipelineWrapper->pipeline = pipeline;
		pipelineWrapper->pipelineType = rhi::PipelineType::Graphics;
		return pipelineWrapper;
	}

	rhi::Framebuffer VkWrapperDevice::CreateFramebuffer(const rhi::FramebufferCreateInfo& info) const {
		auto framebufferWrapper = new VkWrapperFramebuffer();
		std::vector<vk::ImageView> attachments;

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

			framebufferWrapper->colorAttachments.emplace_back(imageView);
		}
		attachments.insert(attachments.end(), framebufferWrapper->colorAttachments.begin(), framebufferWrapper->colorAttachments.end());

		if (info.pDepthStencilAttachment) {
			auto imageViewInfo = vk::ImageViewCreateInfo()
				.setImage(static_cast<VkWrapperImage*>(info.pDepthStencilAttachment->image)->image)
				.setFormat(VkEnumFormat(info.pDepthStencilAttachment->format).Get())
				.setViewType(VkEnumImageViewType(info.pDepthStencilAttachment->viewType).Get())
				.setComponents(vk::ComponentMapping(
					VkEnumComponentSwizzle(info.pDepthStencilAttachment->componentMapping.r).Get(),
					VkEnumComponentSwizzle(info.pDepthStencilAttachment->componentMapping.g).Get(),
					VkEnumComponentSwizzle(info.pDepthStencilAttachment->componentMapping.b).Get(),
					VkEnumComponentSwizzle(info.pDepthStencilAttachment->componentMapping.a).Get()))
				.setSubresourceRange(vk::ImageSubresourceRange(
					vk::ImageAspectFlagBits::eDepth,
					info.pDepthStencilAttachment->baseMipLevel,
					info.pDepthStencilAttachment->mipLevelCount,
					info.pDepthStencilAttachment->baseArrayLayer,
					info.pDepthStencilAttachment->arrayLayerCount));

			auto depthStencilAttachment = device.createImageView(imageViewInfo);

			if (!depthStencilAttachment) {
				throw std::runtime_error("Create image view failed");
			}

			framebufferWrapper->depthStencilAttachment = depthStencilAttachment;
			attachments.emplace_back(depthStencilAttachment);
		}
		
		auto framebufferInfo = vk::FramebufferCreateInfo()
			.setAttachments(attachments)
			.setWidth(info.width)
			.setHeight(info.height)
			.setLayers(info.layers)
			.setRenderPass(static_cast<VkWrapperRenderPass*>(info.renderPass)->renderPass);

		auto framebuffer = device.createFramebuffer(framebufferInfo);

		if (!framebuffer) {
			throw std::runtime_error("Create framebuffer failed");
		}

		framebufferWrapper->device = device;
		framebufferWrapper->framebuffer = framebuffer;
		framebufferWrapper->width = info.width;
		framebufferWrapper->height = info.height;
		framebufferWrapper->layers = info.layers;
		return framebufferWrapper;
	}

	rhi::Fence VkWrapperDevice::CreateFence(const rhi::FenceCreateInfo& info) const {
		auto fenceInfo = vk::FenceCreateInfo();

		auto fence = device.createFence(fenceInfo);
		
		if (!fence) {
			throw std::runtime_error("Create fence failed");
		}

		auto fenceWrapper = new VkWrapperFence();
		fenceWrapper->device = device;
		fenceWrapper->fence = fence;
		return fenceWrapper;
	}


	/* -------------------- VkWrapperQueue -------------------- */


	void VkWrapperQueue::SubmitCommandBuffers(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) {
		std::vector<vk::CommandBuffer> submitCommandBuffers(commandBufferCount);
		for (uint32_t i = 0; i < commandBufferCount; i++) {
			submitCommandBuffers[i] = static_cast<VkWrapperCommandBuffer*>(commandBuffers[i])->commandBuffer;
		}

		auto submitInfo = vk::SubmitInfo()
			.setCommandBuffers(submitCommandBuffers);

		queue.submit(submitInfo, static_cast<VkWrapperFence*>(fence)->fence);
	}

	void VkWrapperQueue::PresentSwapchain(rhi::Swapchain swapchain, uint32_t imageIndex) {
		std::array<uint32_t, 1> imageIndices = { imageIndex };
		std::array<vk::SwapchainKHR, 1> presentSwapchains = { static_cast<VkWrapperSwapchain*>(swapchain)->swapchain };

		auto presentInfo = vk::PresentInfoKHR()
			.setImageIndices(imageIndices)
			.setSwapchains(presentSwapchains);
		
		auto result = queue.presentKHR(presentInfo);
	}


	/* -------------------- VkWrapperSwapchain -------------------- */


	rhi::Extent2D VkWrapperSwapchain::GetImageExtent() const {
		return imageExtent;
	}

	std::vector<rhi::Image> VkWrapperSwapchain::GetImages() const {
		return images;
	}

	uint32_t VkWrapperSwapchain::AcquireNextImage(rhi::Fence fence) {
		auto result = device.acquireNextImageKHR(swapchain, UINT64_MAX, nullptr, static_cast<VkWrapperFence*>(fence)->fence);
		currentImageIndex = result.value;
		return currentImageIndex;
	}
	

	/* -------------------- VkWrapperCommandPool -------------------- */


	void VkWrapperCommandPool::Reset() {
		device.resetCommandPool(commandPool);
		for (auto& commandBuffer : commandBuffers) delete commandBuffer;
		decltype(commandBuffers) temp;
		temp.swap(commandBuffers);
	}

	void VkWrapperCommandPool::Free(uint32_t commandBufferCount, rhi::CommandBuffer* pCommandBuffers) {
		std::vector<vk::CommandBuffer> freeCommandBuffers(commandBufferCount);
		for (uint32_t i = 0; i < commandBufferCount; i++) {
			auto commandBuffer = static_cast<VkWrapperCommandBuffer*>(pCommandBuffers[i]);
			freeCommandBuffers[i] = commandBuffer->commandBuffer;
			commandBuffers[commandBuffer->poolIndex] = nullptr;
			delete commandBuffer;
		}

		device.free(commandPool, freeCommandBuffers);
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

		std::vector<rhi::CommandBuffer> cmdWrappers(bufferCount);
		for (uint32_t i = 0; i < bufferCount; i++) {
			auto cmdWrapper = new VkWrapperCommandBuffer();
			cmdWrapper->commandPool = commandPool;
			cmdWrapper->commandBuffer = cmdBuffer[i];
			cmdWrappers[i] = cmdWrapper;

			bool found = false;
			for (uint32_t j = 0; j < commandBuffers.size(); j++) {
				if (!commandBuffers[j]) {
					commandBuffers[j] = cmdWrapper;
					cmdWrapper->poolIndex = j;
					found = true;
					break;
				}
			}
			if (!found) {
				cmdWrapper->poolIndex = static_cast<uint32_t>(commandBuffers.size());
				commandBuffers.emplace_back(cmdWrapper);
			}
		}
		
		return cmdWrappers;
	}


	/* -------------------- VkWrapperCommandBuffer -------------------- */


	void VkWrapperCommandBuffer::Reset() {
		commandBuffer.reset();
	}

	void VkWrapperCommandBuffer::Begin(const rhi::CommandBufferBeginInfo& info) {
		commandBuffer.begin(vk::CommandBufferBeginInfo());
	}

	void VkWrapperCommandBuffer::End() {
		commandBuffer.end();
	}

	void VkWrapperCommandBuffer::BeginRenderPass(const rhi::RenderPassBeginInfo& info) {
		auto framebuffer = static_cast<VkWrapperFramebuffer*>(info.framebuffer);

		std::vector<vk::ClearValue> clearValues(framebuffer->colorAttachments.size());
		for (uint32_t i = 0; i < framebuffer->colorAttachments.size(); i++) {
			clearValues[i] = vk::ClearColorValue(info.pClearColorValues[i].colorValue);
		}
		if (framebuffer->depthStencilAttachment) {
			clearValues.emplace_back(vk::ClearDepthStencilValue(info.clearDepthStencilValue.depthValue, info.clearDepthStencilValue.stencilValue));
		}

		auto renderPassBeginInfo = vk::RenderPassBeginInfo()
			.setRenderPass(static_cast<VkWrapperRenderPass*>(info.renderPass)->renderPass)
			.setFramebuffer(framebuffer->framebuffer)
			.setRenderArea(vk::Rect2D(vk::Offset2D(0, 0), vk::Extent2D(framebuffer->width, framebuffer->height)))
			.setClearValues(clearValues);
		
		commandBuffer.beginRenderPass(renderPassBeginInfo, vk::SubpassContents::eInline);
	}

	void VkWrapperCommandBuffer::EndRenderPass() {
		commandBuffer.endRenderPass();
	}

	void VkWrapperCommandBuffer::BindPipeline(const rhi::Pipeline& pipeline) {
		auto pipeline_ = static_cast<VkWrapperPipeline*>(pipeline);
		commandBuffer.bindPipeline(VkEnumPipelineBindPoint(pipeline_->pipelineType).Get(), pipeline_->pipeline);
	}

	void VkWrapperCommandBuffer::BindVertexBuffer(uint32_t firstBinding, uint32_t bindingCount, const rhi::Buffer* pBuffers) {
		std::vector<vk::Buffer> vertexBuffers(bindingCount);
		std::vector<vk::DeviceSize> offsets(bindingCount);
		for (uint32_t i = 0; i < bindingCount; i++) {
			vertexBuffers[i] = static_cast<VkWrapperBuffer*>(pBuffers[i])->buffer;
			offsets[i] = 0;
		}
		
		commandBuffer.bindVertexBuffers(firstBinding, bindingCount, vertexBuffers.data(), offsets.data());
	}

	void VkWrapperCommandBuffer::BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) {
		auto indexBuffer = static_cast<VkWrapperBuffer*>(buffer)->buffer;
		commandBuffer.bindIndexBuffer(indexBuffer, offset, VkEnumIndexType(indexType).Get());
	}

	void VkWrapperCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
		commandBuffer.draw(vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void VkWrapperCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
		commandBuffer.drawIndexed(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}


	/* -------------------- VkWrapperBuffer -------------------- */


	void* VkWrapperBuffer::Map() {
		auto pData = device.mapMemory(memory, 0, VK_WHOLE_SIZE);

		if (!pData) {
			throw std::runtime_error("Map the memory failed");
		}

		return pData;
	}

	void* VkWrapperBuffer::Map(uint64_t offset, uint64_t size) {
		auto pData = device.mapMemory(memory, offset, size);

		if (!pData) {
			throw std::runtime_error("Map the memory failed");
		}

		return pData;
	}

	void VkWrapperBuffer::Unmap() {
		device.unmapMemory(memory);
	}


	/* -------------------- VkWrapperDescriptorPool -------------------- */


	void VkWrapperDescriptorPool::Reset() {
		device.resetDescriptorPool(descriptorPool);
		for (auto& descriptorSet : descriptorSets) {
			delete descriptorSet;
			descriptorSet = nullptr;
		}
	}

	void VkWrapperDescriptorPool::Free(uint32_t descriptorCount, rhi::DescriptorSet* pDescriptorSets) {
		std::vector<vk::DescriptorSet> freeDescriptorSets(descriptorCount);
		for (uint32_t i = 0; i < descriptorCount; i++) {
			auto descriptorSet = static_cast<VkWrapperDescriptorSet*>(pDescriptorSets[i]);
			freeDescriptorSets[i] = descriptorSet->descriptorSet;
			descriptorSets[descriptorSet->poolIndex] = nullptr;
			delete descriptorSet;
		}

		device.free(descriptorPool, freeDescriptorSets);
	}

	std::vector<rhi::DescriptorSet> VkWrapperDescriptorPool::AllocateDescriptorSets(const rhi::DescriptorSetAllocateInfo& info) {
		std::vector<vk::DescriptorSetLayout> descriptorSetLayouts(info.descriptorSetCount);
		for (uint32_t i = 0; i < info.descriptorSetCount; i++) {
			descriptorSetLayouts[i] = static_cast<VkWrapperDescriptorSetLayout*>(info.pDescriptorSetLayouts[i])->descriptorSetLayout;
		}
		
		auto descriptorSetAllocInfo = vk::DescriptorSetAllocateInfo()
			.setDescriptorPool(descriptorPool)
			.setSetLayouts(descriptorSetLayouts);

		auto descriptorSets = device.allocateDescriptorSets(descriptorSetAllocInfo);

		if (descriptorSets.empty()) {
			throw std::runtime_error("Allocate descriptor sets failed");
		}

		std::vector<rhi::DescriptorSet> descriptorSetWrappers(info.descriptorSetCount);
		for (uint32_t i = 0; i < info.descriptorSetCount; i++) {
			auto descriptorSetWrapper = new VkWrapperDescriptorSet();
			descriptorSetWrapper->device = device;
			descriptorSetWrapper->descriptorSet = descriptorSets[i];
			descriptorSetWrappers[i] = descriptorSetWrapper;

			bool found = false;
			for (uint32_t j = 0; j < this->descriptorSets.size(); j++) {
				if (!this->descriptorSets[j]) {
					this->descriptorSets[j] = descriptorSetWrapper;
					descriptorSetWrapper->poolIndex = j;
					found = true;
					break;
				}
			}
			if (!found) {
				throw std::runtime_error("No enough pool space for descriptor sets");
			}
		}

		return descriptorSetWrappers;
	}


	/* -------------------- VkWrapperDescriptorSet -------------------- */


	void VkWrapperDescriptorSet::Write(uint32_t dstSet, uint32_t dstBinding, rhi::DescriptorType, rhi::Buffer) {

	}

	void VkWrapperDescriptorSet::Write(uint32_t dstSet, uint32_t dstBinding, rhi::DescriptorType, rhi::ImageViewInfo) {

	}


	/* -------------------- VkWrapperFence -------------------- */


	void VkWrapperFence::Reset() {
		auto result = device.resetFences(1, &fence);
	}

	void VkWrapperFence::Wait() {
		vk::Result result;
		do result = device.waitForFences(VK_TRUE, &fence, VK_TRUE, UINT64_MAX);
		while (result == vk::Result::eTimeout);
	}

	bool VkWrapperFence::GetCurrentStatus() const {
		auto result = device.getFenceStatus(fence);
		if (result == vk::Result::eSuccess) return true;
		return false;
	}


	/* -------------------- destroy handle -------------------- */


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
		instance.destroy(surface);
		for (auto& image : images) delete image;
		delete this;
	}

	void VkWrapperCommandPool::Destroy() {
		device.destroy(commandPool);
		for (auto& cmdBuffer : commandBuffers) {
			if (cmdBuffer) delete cmdBuffer;
		}
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

	void VkWrapperSampler::Destroy() {
		device.destroy(sampler);
		delete this;
	}

	void VkWrapperDescriptorPool::Destroy() {
		device.destroy(descriptorPool);
		for (auto& descriptorSet : descriptorSets) {
			if (descriptorSet) delete descriptorSet;
		}
		delete this;
	}

	void VkWrapperDescriptorSetLayout::Destroy() {
		device.destroy(descriptorSetLayout);
		delete this;
	}

	void VkWrapperPipelineLayout::Destroy() {
		device.destroy(pipelineLayout);
		delete this;
	}

	void VkWrapperRenderPass::Destroy() {
		device.destroy(renderPass);
		delete this;
	}

	void VkWrapperShaderModule::Destroy() {
		device.destroy(shaderModule);
		delete this;
	}

	void VkWrapperPipeline::Destroy() {
		device.destroy(pipeline);
		delete this;
	}

	void VkWrapperFramebuffer::Destroy() {
		device.destroy(framebuffer);
		for (auto& colorAttachment : colorAttachments) device.destroy(colorAttachment);
		if (depthStencilAttachment) device.destroy(depthStencilAttachment);
		delete this;
	}

	void VkWrapperFence::Destroy() {
		device.destroy(fence);
		delete this;
	}

}

#endif // _VULKAN