#include "vulkan_rhi.hpp"

namespace engine {

	void VulkanInstance::Init(const RHIInstanceInitInfo& info) {
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
			.setMessageSeverity(vk::DebugUtilsMessageSeverityFlagBitsEXT::eVerbose | vk::DebugUtilsMessageSeverityFlagBitsEXT::eWarning | vk::DebugUtilsMessageSeverityFlagBitsEXT::eError)
			.setMessageType(vk::DebugUtilsMessageTypeFlagBitsEXT::eGeneral | vk::DebugUtilsMessageTypeFlagBitsEXT::eValidation | vk::DebugUtilsMessageTypeFlagBitsEXT::ePerformance)
			.setPfnUserCallback(reinterpret_cast<PFN_vkDebugUtilsMessengerCallbackEXT>(VulkanUtil::DebugCallback))
			.setPUserData(nullptr);

		vk::DispatchLoaderDynamic dispatch(instance, GetInstanceProcAddr);
		debugMessenger = instance.createDebugUtilsMessengerEXT(debugMessengerInfo, nullptr, dispatch);
		
		if (!debugMessenger) {
			throw std::runtime_error("Create debug messenger failed");
		}
#endif // _DEBUG
	}

	void VulkanInstance::CreateInstance(const RHIInstanceInitInfo& info) {
		auto applicationInfo = vk::ApplicationInfo()
			.setApiVersion(VK_API_VERSION_1_3)
			.setPEngineName("Verse Engine")
			.setEngineVersion(1)
			.setPApplicationName(info.applicationName)
			.setApplicationVersion(info.applicationVersion);

		auto instanceInfo = vk::InstanceCreateInfo()
			.setPEnabledExtensionNames(extensions.device)
			.setPApplicationInfo(&applicationInfo)
			.setPEnabledLayerNames(validationLayers);

		instance = vk::createInstance(instanceInfo);

		if (!instance) {
			throw std::runtime_error("Create instance failed");
		}
	}

	void VulkanInstance::Destroy() {
#ifdef _DEBUG
		instance.destroy(debugMessenger);
#endif // _DEBUG

		instance.destroy();
	}

	RHIDevice VulkanInstance::CreateDevice(const RHIDeviceCreateInfo& info) const {
		auto gpus = instance.enumeratePhysicalDevices();

		if (info.physicalDeviceID >= gpus.size()) {
			throw std::runtime_error("Cannot find chosen physical device");
		}

		auto physicalDevice = gpus[info.physicalDeviceID];
		auto queueProp = physicalDevice.getQueueFamilyProperties();

		std::vector<vk::DeviceQueueCreateInfo> deviceQueueInfos;

		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			uint32_t queueIndex;
			bool found = false;
			for (uint32_t j = 0; j < queueProp.size(); j++) {
				if (queueProp[j].queueFlags & VkEnumQueueType(info.pQueueCreateInfo[i].queueType).Get()) {
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
			.setGeometryShader(info.deviceFeature.geometryShader);

		auto deviceExtensions = extensions.device;

		if (info.deviceFeature.hardwareVideoAcceleration) {
			deviceExtensions.push_back(VK_KHR_VIDEO_DECODE_QUEUE_EXTENSION_NAME);
			deviceExtensions.push_back(VK_KHR_VIDEO_ENCODE_QUEUE_EXTENSION_NAME);
		}

		auto deviceInfo = vk::DeviceCreateInfo()
			.setQueueCreateInfos(deviceQueueInfos)
			.setPEnabledExtensionNames(extensions.device)
			.setPEnabledFeatures(&feature);

		auto device = physicalDevice.createDevice(deviceInfo);

		if (!device) {
			throw std::runtime_error("Create logical device failed");
		}

		std::vector<RHIQueue> queues;
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			auto queue = new VkWrapperQueue();
			queue->SetQueueFamilyIndex(deviceQueueInfos[i].queueFamilyIndex).SetQueue(device.getQueue(deviceQueueInfos[i].queueFamilyIndex, 0));
			queues.push_back(queue);
		}

		auto deviceWrapper = new VkWrapperDevice();
		deviceWrapper->SetPhysicalDevice(physicalDevice).SetDevice(device).SetQueues(queues);
		return deviceWrapper;
	}
	
	RHISwapchain VulkanInstance::CreateSwapchain(RHIDevice& device, const RHISwapchainCreateInfo& info) const {
		vk::SurfaceKHR surface;

#ifdef _WIN32
		auto surfaceInfo = vk::Win32SurfaceCreateInfoKHR()
			.setHwnd(info.platformInfo.hWnd)
			.setHinstance(info.platformInfo.hInstance);

		surface = instance.createWin32SurfaceKHR(surfaceInfo);
#endif // _WIN32

		if (!surface) {
			throw std::runtime_error("Create surface failed");
		}

		auto physicalDevice = static_cast<VkWrapperDevice*>(device)->GetPhysicalDevice();

		auto surfaceFormats = physicalDevice.getSurfaceFormats2KHR(surface);
		if (!surfaceFormats.size()) {
			throw std::runtime_error("Do not support any surface format");
		}

		auto surfacePresentModes = physicalDevice.getSurfacePresentModesKHR(surface);
		if (!surfacePresentModes.size()) {
			throw std::runtime_error("Do not support any surface present mode");
		}

		auto surfaceCapabilities = physicalDevice.getSurfaceCapabilitiesKHR(surface);

		auto swapchainInfo = vk::SwapchainCreateInfoKHR()
			.setSurface(surface)
			.setImageFormat(VkEnumFormat(info.format).Get())
			.setMinImageCount(info.frameCount)
			.setImageExtent(vk::Extent2D(info.imageExtent.width, info.imageExtent.height))
			.setPreTransform(surfaceCapabilities.currentTransform)
			.setPresentMode(vk::PresentModeKHR::eFifo)
			.setImageSharingMode(vk::SharingMode::eExclusive)
			.setCompositeAlpha(VkEnumAlphaMode(info.alphaMode).Get())
			.setImageColorSpace(surfaceFormats[0].surfaceFormat.colorSpace)
			.setImageUsage(VkEnumImageUsage(info.imageUsage).Get())
			.setImageArrayLayers(1)
			.setClipped(true);

		auto swapchain = static_cast<VkWrapperDevice*>(device)->GetDevice().createSwapchainKHR(swapchainInfo);

		if (!swapchain) {
			throw std::runtime_error("Create swapchain failed");
		}
		
		auto swapchainImages = static_cast<VkWrapperDevice*>(device)->GetDevice().getSwapchainImagesKHR(swapchain);
		std::vector<vk::ImageView> swapchainImageViews(info.frameCount);

		for (size_t i = 0; i < info.frameCount; i++) {
			auto imageViewInfo = vk::ImageViewCreateInfo()
				.setViewType(vk::ImageViewType::e2D)
				.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1))
				.setFormat(VkEnumFormat(info.format).Get())
				.setImage(swapchainImages[i])
				.setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity));

			auto swapchainImageView = static_cast<VkWrapperDevice*>(device)->GetDevice().createImageView(imageViewInfo);

			if (!swapchainImageView) {
				throw std::runtime_error("Create swapchain image view failed");
			}

			swapchainImageViews.push_back(swapchainImageView);
		}

		auto swapchainWrapper = new VkWrapperSwapchain();
		swapchainWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetSurface(surface).SetSwapchain(swapchain).SetImageViews(swapchainImageViews);
		return swapchainWrapper;
	}
	
	RHICommandPool VulkanInstance::CreateCommandPool(RHIDevice& device, const RHICommandPoolCreateInfo& info) const {
		auto cmdPoolInfo = vk::CommandPoolCreateInfo()
			.setQueueFamilyIndex(static_cast<VkWrapperQueue*>(info.queue)->GetQueueFamilyIndex())
			.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

		auto cmdPool = static_cast<const VkWrapperDevice*>(device)->GetDevice().createCommandPool(cmdPoolInfo);

		if (!cmdPool) {
			throw std::runtime_error("Create command pool failed");
		}

		auto cmdPoolWrapper = new VkWrapperCommandPool();
		cmdPoolWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetCommandPool(cmdPool);
		return cmdPoolWrapper;
	}

	RHIBuffer VulkanInstance::CreateBuffer(RHIDevice& device, const RHIBufferCreateInfo& info) const {
		auto bufferInfo = vk::BufferCreateInfo()
			.setSize(info.size)
			.setUsage(VkEnumBufferUsage(info.usage).Get());

		auto buffer = static_cast<VkWrapperDevice*>(device)->GetDevice().createBuffer(bufferInfo);

		if (!buffer) {
			throw std::runtime_error("Create buffer failed");
		}

		auto memReqs = static_cast<VkWrapperDevice*>(device)->GetDevice().getBufferMemoryRequirements(buffer);
		auto memoryInfo = vk::MemoryAllocateInfo()
			.setAllocationSize(memReqs.size);
		VulkanUtil::MemoryTypeFromProperties(static_cast<VkWrapperDevice*>(device)->GetPhysicalDevice().getMemoryProperties(), memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eHostCoherent | vk::MemoryPropertyFlagBits::eHostVisible, memoryInfo.memoryTypeIndex);

		auto memory = static_cast<VkWrapperDevice*>(device)->GetDevice().allocateMemory(memoryInfo);

		if (!memory) {
			throw std::runtime_error("Allocate memory failed");
		}

		static_cast<VkWrapperDevice*>(device)->GetDevice().bindBufferMemory(buffer, memory, 0);

		auto bufferWrapper = new VkWrapperBuffer();
		bufferWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetBuffer(buffer).SetMemory(memory);
		return bufferWrapper;
	}

	RHIBufferView VulkanInstance::CreateBufferView(RHIDevice& device, const RHIBufferViewCreateInfo& info) const {
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
	}

	RHIImage VulkanInstance::CreateImage(RHIDevice& device, const RHIImageCreateInfo& info) const {
		auto imageInfo = vk::ImageCreateInfo()
			.setFormat(VkEnumFormat(info.format).Get())
			.setImageType(VkEnumImageType(info.imageType).Get())
			.setExtent(vk::Extent3D(info.extent.width, info.extent.height, info.extent.depth))
			.setUsage(VkEnumImageUsage(info.usage).Get())
			.setTiling(vk::ImageTiling::eOptimal)
			.setArrayLayers(1)
			.setMipLevels(info.mipLevels)
			.setInitialLayout(vk::ImageLayout::eUndefined)
			.setSamples(VkEnumSampleCount(info.sampleCount).Get());
		
		auto image = static_cast<VkWrapperDevice*>(device)->GetDevice().createImage(imageInfo);

		if (!image) {
			throw std::runtime_error("Create image failed");
		}

		auto memReqs = static_cast<VkWrapperDevice*>(device)->GetDevice().getImageMemoryRequirements(image);
		auto memoryInfo = vk::MemoryAllocateInfo()
			.setAllocationSize(memReqs.size);
		VulkanUtil::MemoryTypeFromProperties(static_cast<VkWrapperDevice*>(device)->GetPhysicalDevice().getMemoryProperties(), memReqs.memoryTypeBits, vk::MemoryPropertyFlagBits::eDeviceLocal, memoryInfo.memoryTypeIndex);

		auto memory = static_cast<VkWrapperDevice*>(device)->GetDevice().allocateMemory(memoryInfo);

		if (!memory) {
			throw std::runtime_error("Allocate memory failed");
		}

		static_cast<VkWrapperDevice*>(device)->GetDevice().bindImageMemory(image, memory, 0);

		auto imageWrapper = new VkWrapperImage();
		imageWrapper->SetDevice(static_cast<VkWrapperDevice*>(device)->GetDevice()).SetImage(image).SetMemory(memory);
		return imageWrapper;
	}

	RHIImageView VulkanInstance::CreateImageView(RHIDevice& device, const RHIImageViewCreateInfo& info) const {
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
	}

	RHIDescriptorPool VulkanInstance::CreateDescriptorPool(RHIDevice& device, const RHIDescriptorPoolCreateInfo& info) const {
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
	}

	RHIPipeline VulkanInstance::CreateGraphicsPipeline(RHIDevice& device, const RHIGraphicsPipelineCreateInfo& info) const {
		
	}

	void VulkanInstance::Destroy(RHIDevice& device) const {
		static_cast<VkWrapperDevice*>(device)->GetDevice().destroy();
		for (auto queue : static_cast<VkWrapperDevice*>(device)->GetQueues()) delete queue;
		delete device;
	}

	void VulkanInstance::Destroy(RHISwapchain& swapchain) const {
		instance.destroy(static_cast<VkWrapperSwapchain*>(swapchain)->GetSurface());
		static_cast<VkWrapperSwapchain*>(swapchain)->GetDevice().destroy(static_cast<VkWrapperSwapchain*>(swapchain)->GetSwapchain());
		
		for (auto& imageView : static_cast<VkWrapperSwapchain*>(swapchain)->GetImageViews()) {
			static_cast<VkWrapperSwapchain*>(swapchain)->GetDevice().destroy(imageView);
		}
		
		delete swapchain;
	}

	void VulkanInstance::Destroy(RHICommandPool& cmdPool) const {
		static_cast<VkWrapperCommandPool*>(cmdPool)->GetDevice().destroy(static_cast<VkWrapperCommandPool*>(cmdPool)->GetCommandPool());
		for (auto& cmdBuffer : static_cast<VkWrapperCommandPool*>(cmdPool)->GetCommandBuffers()) {
			delete cmdBuffer;
		}
		delete cmdPool;
	}

	void VulkanInstance::Destroy(RHIBuffer& buffer) const {
		static_cast<VkWrapperBuffer*>(buffer)->GetDevice().destroy(static_cast<VkWrapperBuffer*>(buffer)->GetBuffer());
		static_cast<VkWrapperBuffer*>(buffer)->GetDevice().free(static_cast<VkWrapperBuffer*>(buffer)->GetMemory());
		delete buffer;
	}

	void VulkanInstance::Destroy(RHIBufferView& bufferView) const {
		static_cast<VkWrapperBufferView*>(bufferView)->GetDevice().destroy(static_cast<VkWrapperBufferView*>(bufferView)->GetBufferView());
		delete bufferView;
	}

	void VulkanInstance::Destroy(RHIImage& image) const {
		static_cast<VkWrapperImage*>(image)->GetDevice().destroy(static_cast<VkWrapperImage*>(image)->GetImage());
		static_cast<VkWrapperImage*>(image)->GetDevice().free(static_cast<VkWrapperImage*>(image)->GetMemory());
		delete image;
	}

	void VulkanInstance::Destroy(RHIImageView& imageView) const {
		static_cast<VkWrapperImageView*>(imageView)->GetDevice().destroy(static_cast<VkWrapperImageView*>(imageView)->GetImageView());
		delete imageView;
	}

	void VulkanInstance::Destroy(RHIDescriptorPool& descriptorPool) const {
		static_cast<VkWrapperDescriptorPool*>(descriptorPool)->GetDevice().destroy(static_cast<VkWrapperDescriptorPool*>(descriptorPool)->GetDescriptorPool());
		delete descriptorPool;
	}

	RHIQueue VulkanInstance::GetQueue(RHIDevice& device, uint32_t queueIndex) const {
		auto queues = static_cast<VkWrapperDevice*>(device)->GetQueues();
		if (queueIndex >= queues.size()) {
			throw std::runtime_error("Queue index out of range");
		}
		return queues[queueIndex];
	}

	std::vector<RHICommandBuffer> VulkanInstance::AllocateCommandBuffers(RHIDevice& device, const RHICommandBufferAllocateInfo& info) const {
		auto cmdAllocInfo = vk::CommandBufferAllocateInfo()
			.setCommandPool(static_cast<VkWrapperCommandPool*>(info.commandPool)->GetCommandPool())
			.setLevel(vk::CommandBufferLevel::ePrimary)
			.setCommandBufferCount(info.commandBufferCount);

		auto cmdBuffer = static_cast<VkWrapperDevice*>(device)->GetDevice().allocateCommandBuffers(cmdAllocInfo);

		if (!cmdBuffer.size()) {
			throw std::runtime_error("Allocate command buffers failed");
		}

		std::vector<RHICommandBuffer> cmdWrappers;
		for (uint32_t i = 0; i < info.commandBufferCount; i++) {
			auto cmdWrapper = new VkWrapperCommandBuffer();
			cmdWrapper->SetCommandBuffer(cmdBuffer[i]);
			cmdWrappers.push_back(cmdWrapper);
		}

		auto poolCmdBuffers = static_cast<VkWrapperCommandPool*>(info.commandPool)->GetCommandBuffers();
		poolCmdBuffers.insert(poolCmdBuffers.end(), cmdWrappers.begin(), cmdWrappers.end());
		static_cast<VkWrapperCommandPool*>(info.commandPool)->SetCommandBuffers(poolCmdBuffers);

		return cmdWrappers;
	}

	std::vector<RHIPhysicalDeviceInfo> VulkanInstance::EnumeratePhysicalDevice() const {
		auto gpus = instance.enumeratePhysicalDevices();

		if (!gpus.size()) {
			throw std::runtime_error("Enumerate physical devices failed");
		}

		std::vector<RHIPhysicalDeviceInfo> infos;

		for (auto& gpu : gpus) {
			auto properties = gpu.getProperties();

			auto info = RHIPhysicalDeviceInfo()
				.SetDeviceID(properties.deviceID)
				.SetDeviceName(properties.deviceName)
				.SetApiVersion(properties.apiVersion);

			infos.push_back(info);
		}
		return infos;
	}

}