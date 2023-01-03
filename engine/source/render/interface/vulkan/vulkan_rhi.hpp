#pragma once

#include "../rhi.hpp"
#include "vulkan_base.hpp"

namespace engine {

	class VulkanInstance final : public RHIInstance {
	public:
		void Init(const RHIInstanceInitInfo&) override;
		void Destroy() override;
		
		// create
		RHIDevice CreateDevice(const RHIDeviceCreateInfo&) const override;
		RHISwapchain CreateSwapchain(RHIDevice&, const RHISwapchainCreateInfo&) const override;
		RHICommandPool CreateCommandPool(RHIDevice&, const RHICommandPoolCreateInfo&) const override;
		RHIBuffer CreateBuffer(RHIDevice&, const RHIBufferCreateInfo&) const override;
		RHIBufferView CreateBufferView(RHIDevice&, const RHIBufferViewCreateInfo&) const override;
		RHIImage CreateImage(RHIDevice&, const RHIImageCreateInfo&) const override;
		RHIPipeline CreateGraphicsPipeline(RHIDevice&, const RHIGraphicsPipelineCreateInfo&) const override;

		// destroy
		void Destroy(RHIDevice&) const override;
		void Destroy(RHISwapchain&) const override;
		void Destroy(RHICommandPool&) const override;
		void Destroy(RHIBuffer&) const override;
		void Destroy(RHIBufferView&) const override;
		void Destroy(RHIImage&) const override;
		void Destroy(RHIPipeline&) const override;

		// get
		RHIQueue GetQueue(RHIDevice&, uint32_t queueIndex) const override;
		std::vector<RHICommandBuffer> AllocateCommandBuffers(RHIDevice&, const RHICommandBufferAllocateInfo&) const override;
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