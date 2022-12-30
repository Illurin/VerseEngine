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
		uint32_t graphicsQueueFamilyIndex = 0;

		bool found = false;
		for (size_t i = 0; i < queueProp.size(); i++) {
			if (queueProp[i].queueFlags & vk::QueueFlagBits::eGraphics) {
				graphicsQueueFamilyIndex = i;
				found = true;
				break;
			}
		}
		if (!found) {
			throw std::runtime_error("Cannot find queue family");
		}

		auto feature = vk::PhysicalDeviceFeatures()
			.setGeometryShader(VK_TRUE);

		std::array<float, 1> priorities = { 0.0f };
		auto queueInfo = vk::DeviceQueueCreateInfo()
			.setQueueCount(1)
			.setPQueuePriorities(priorities.data())
			.setQueueFamilyIndex(graphicsQueueFamilyIndex);

		auto deviceInfo = vk::DeviceCreateInfo()
			.setQueueCreateInfoCount(1)
			.setPQueueCreateInfos(&queueInfo)
			.setEnabledExtensionCount(extensions.device.size())
			.setPpEnabledExtensionNames(extensions.device.data())
			.setPEnabledFeatures(&feature);

		auto device = physicalDevice.createDevice(deviceInfo);

		if (!device) {
			throw std::runtime_error("Create logical device failed");
		}

		auto deviceWrapper = new RHIDevice();
		static_cast<VkWrapperDevice*>(deviceWrapper)->SetPhysicalDevice(physicalDevice).SetDevice(device).SetQueueFamilyIndex(graphicsQueueFamilyIndex);
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

		auto queueFamilyIndex = static_cast<const VkWrapperDevice*>(device)->GetQueueFamilyIndex();

		if (!physicalDevice.getSurfaceSupportKHR(queueFamilyIndex, surface)) {
			throw std::runtime_error("The queue family do not support present");
		}

		auto swapchainInfo = vk::SwapchainCreateInfoKHR()
			.setSurface(surface)
			.setImageFormat(VkEnumFormat(info.format).Get())
			.setMinImageCount(info.frameCount)
			.setImageExtent(vk::Extent2D(info.imageExtent.width, info.imageExtent.height))
			.setPreTransform(surfaceCapabilities.currentTransform)
			.setPresentMode(vk::PresentModeKHR::eFifo)
			.setImageSharingMode(vk::SharingMode::eExclusive)
			.setCompositeAlpha(vk::CompositeAlphaFlagBitsKHR::eOpaque)
			.setImageColorSpace(surfaceFormats[0].surfaceFormat.colorSpace)
			.setImageUsage(vk::ImageUsageFlagBits::eColorAttachment)
			.setImageArrayLayers(1)
			.setClipped(true);

		auto swapchain = static_cast<const VkWrapperDevice*>(device)->GetDevice().createSwapchainKHR(swapchainInfo);

		if (!swapchain) {
			throw std::runtime_error("Create swapchain failed");
		}

		auto swapchainWrapper = new RHISwapchain();
		static_cast<VkWrapperSwapchain*>(swapchainWrapper)->SetSurface(surface).SetSwapchain(swapchain);
		return swapchainWrapper;
	}
	
	void VulkanInstance::DestroyDevice(RHIDevice* device) const {
		static_cast<VkWrapperDevice*>(device)->GetDevice().destroy();
		delete device;
	}

	void VulkanInstance::DestroySwapchain(const RHIDevice* device, RHISwapchain* swapchain) const {
		instance.destroy(static_cast<VkWrapperSwapchain*>(swapchain)->GetSurface());
		static_cast<const VkWrapperDevice*>(device)->GetDevice().destroy(static_cast<VkWrapperSwapchain*>(swapchain)->GetSwapchain());
		delete swapchain;
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

	void VulkanInstance::CreateInstance(const RHIInstanceInitInfo& info) {
		auto applicationInfo = vk::ApplicationInfo()
			.setApiVersion(VK_API_VERSION_1_3)
			.setPEngineName("Verse Engine")
			.setEngineVersion(1)
			.setPApplicationName(info.applicationName)
			.setApplicationVersion(info.applicationVersion);

		auto instanceInfo = vk::InstanceCreateInfo()
			.setEnabledExtensionCount(extensions.instance.size())
			.setPpEnabledExtensionNames(extensions.device.data())
			.setPApplicationInfo(&applicationInfo)
			.setEnabledLayerCount(validationLayers.size())
			.setPpEnabledLayerNames(validationLayers.data());

		instance = vk::createInstance(instanceInfo);

		if (!instance) {
			throw std::runtime_error("Create instance failed");
		}
	}

}