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

	RHIDevice* VulkanInstance::CreateDevice(const RHIDeviceCreateInfo& info) const {
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

		std::vector<RHIQueue*> queues;
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			auto queue = new RHIQueue();
			static_cast<VkWrapperQueue*>(queue)->SetQueueFamilyIndex(deviceQueueInfos[i].queueFamilyIndex).SetQueue(device.getQueue(deviceQueueInfos[i].queueFamilyIndex, 0));
			queues.push_back(queue);
		}

		auto deviceWrapper = new RHIDevice();
		static_cast<VkWrapperDevice*>(deviceWrapper)->SetPhysicalDevice(physicalDevice).SetDevice(device).SetQueues(queues);
		return deviceWrapper;
	}
	
	RHISwapchain* VulkanInstance::CreateSwapchain(const RHIDevice* device, const RHISwapchainCreateInfo& info) const {
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

		auto physicalDevice = static_cast<const VkWrapperDevice*>(device)->GetPhysicalDevice();

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

		auto swapchain = static_cast<const VkWrapperDevice*>(device)->GetDevice().createSwapchainKHR(swapchainInfo);

		if (!swapchain) {
			throw std::runtime_error("Create swapchain failed");
		}

		auto swapchainImages = static_cast<const VkWrapperDevice*>(device)->GetDevice().getSwapchainImagesKHR(swapchain);
		std::vector<vk::ImageView> swapchainImageViews(info.frameCount);

		for (size_t i = 0; i < info.frameCount; i++) {
			auto imageViewInfo = vk::ImageViewCreateInfo()
				.setViewType(vk::ImageViewType::e2D)
				.setSubresourceRange(vk::ImageSubresourceRange(vk::ImageAspectFlagBits::eColor, 0, 1, 0, 1))
				.setFormat(VkEnumFormat(info.format).Get())
				.setImage(swapchainImages[i])
				.setComponents(vk::ComponentMapping(vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity, vk::ComponentSwizzle::eIdentity));

			auto swapchainImageView = static_cast<const VkWrapperDevice*>(device)->GetDevice().createImageView(imageViewInfo);

			if (!swapchainImageView) {
				throw std::runtime_error("Create swapchain image view failed");
			}

			swapchainImageViews.push_back(swapchainImageView);
		}

		auto swapchainWrapper = new RHISwapchain();
		static_cast<VkWrapperSwapchain*>(swapchainWrapper)->SetSurface(surface).SetSwapchain(swapchain).SetImageViews(swapchainImageViews);
		return swapchainWrapper;
	}
	
	RHICommandPool* VulkanInstance::CreateCommandPool(const RHIDevice* device, const RHICommandPoolCreateInfo& info) const {
		auto cmdPoolInfo = vk::CommandPoolCreateInfo()
			.setQueueFamilyIndex(static_cast<const VkWrapperQueue*>(info.queue)->GetQueueFamilyIndex())
			.setFlags(vk::CommandPoolCreateFlagBits::eResetCommandBuffer);

		auto cmdPool = static_cast<const VkWrapperDevice*>(device)->GetDevice().createCommandPool(cmdPoolInfo);

		if (!cmdPool) {
			throw std::runtime_error("Create command pool failed");
		}

		auto cmdPoolWrapper = new RHICommandPool();
		static_cast<VkWrapperCommandPool*>(cmdPoolWrapper)->SetCommandPool(cmdPool);
		return cmdPoolWrapper;
	}

	void VulkanInstance::DestroyDevice(RHIDevice* device) const {
		static_cast<VkWrapperDevice*>(device)->GetDevice().destroy();
		for (auto queue : static_cast<VkWrapperDevice*>(device)->GetQueues()) delete queue;
		delete device;
	}

	void VulkanInstance::DestroySwapchain(const RHIDevice* device, RHISwapchain* swapchain) const {
		instance.destroy(static_cast<VkWrapperSwapchain*>(swapchain)->GetSurface());
		static_cast<const VkWrapperDevice*>(device)->GetDevice().destroy(static_cast<VkWrapperSwapchain*>(swapchain)->GetSwapchain());
		
		for (auto& imageView : static_cast<VkWrapperSwapchain*>(swapchain)->GetImageViews()) {
			static_cast<const VkWrapperDevice*>(device)->GetDevice().destroy(imageView);
		}
		
		delete swapchain;
	}

	void VulkanInstance::DestroyCommandPool(const RHIDevice* device, RHICommandPool* cmdPool) const {
		static_cast<const VkWrapperDevice*>(device)->GetDevice().destroy(static_cast<VkWrapperCommandPool*>(cmdPool)->GetCommandPool());
		for (auto& cmdBuffer : static_cast<VkWrapperCommandPool*>(cmdPool)->GetCommandBuffers()) {
			delete cmdBuffer;
		}
		delete cmdPool;
	}

	RHIQueue* VulkanInstance::GetQueue(const RHIDevice* device, uint32_t queueIndex) const {
		auto queues = static_cast<const VkWrapperDevice*>(device)->GetQueues();
		if (queueIndex >= queues.size()) {
			throw std::runtime_error("Queue index out of range");
		}
		return queues[queueIndex];
	}

	std::vector<RHICommandBuffer*> VulkanInstance::AllocateCommandBuffers(const RHIDevice* device, const RHICommandBufferAllocateInfo& info) const {
		auto cmdAllocInfo = vk::CommandBufferAllocateInfo()
			.setCommandPool(static_cast<const VkWrapperCommandPool*>(info.commandPool)->GetCommandPool())
			.setLevel(vk::CommandBufferLevel::ePrimary)
			.setCommandBufferCount(info.commandBufferCount);

		auto cmdBuffer = static_cast<const VkWrapperDevice*>(device)->GetDevice().allocateCommandBuffers(cmdAllocInfo);

		if (!cmdBuffer.size()) {
			throw std::runtime_error("Allocate command buffers failed");
		}

		std::vector<RHICommandBuffer*> cmdWrappers;
		for (uint32_t i = 0; i < info.commandBufferCount; i++) {
			auto cmdWrapper = new RHICommandBuffer();
			static_cast<VkWrapperCommandBuffer*>(cmdWrapper)->SetCommandBuffer(cmdBuffer[i]);
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