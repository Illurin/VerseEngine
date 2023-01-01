#pragma once

#include "../rhi.hpp"
#include "vulkan_base.hpp"

namespace engine {

	class VulkanInstance final : public RHIInstance {
	public:
		void Init(const RHIInstanceInitInfo&) override;
		void Destroy() override;
		
		// create
		RHIDevice* CreateDevice(const RHIDeviceCreateInfo&) const override;
		RHISwapchain* CreateSwapchain(const RHIDevice*, const RHISwapchainCreateInfo&) const override;
		RHICommandPool* CreateCommandPool(const RHIDevice*, const RHICommandPoolCreateInfo&) const override;

		// destroy
		void DestroyDevice(RHIDevice*) const override;
		void DestroySwapchain(const RHIDevice*, RHISwapchain*) const override;
		void DestroyCommandPool(const RHIDevice*, RHICommandPool*) const override;

		// get
		RHIQueue* GetQueue(const RHIDevice*, uint32_t queueIndex) const override;
		std::vector<RHICommandBuffer*> AllocateCommandBuffers(const RHIDevice*, const RHICommandBufferAllocateInfo&) const override;
		std::vector<RHIPhysicalDeviceInfo> EnumeratePhysicalDevice() const override;

		// command

	private:
		void CreateInstance(const RHIInstanceInitInfo&);

		vk::Instance instance{ nullptr };

		struct {
			std::vector<const char*> instance;
			std::vector<const char*> device;
		}extensions;

		std::vector<const char*> validationLayers;

#ifdef _DEBUG
		vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
#endif // _DEBUG
	};

}