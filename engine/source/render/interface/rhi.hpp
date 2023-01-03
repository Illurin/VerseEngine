#pragma once

#include "rhi_base.hpp"

namespace engine {

	class RHIInstance {
	public:
		virtual void Init(const RHIInstanceInitInfo&) = 0;
		virtual void Destroy() = 0;

		// create
		virtual RHIDevice CreateDevice(const RHIDeviceCreateInfo&) const = 0;
		virtual RHISwapchain CreateSwapchain(RHIDevice&, const RHISwapchainCreateInfo&) const = 0;
		virtual RHICommandPool CreateCommandPool(RHIDevice&, const RHICommandPoolCreateInfo&) const = 0;
		virtual RHIBuffer CreateBuffer(RHIDevice&, const RHIBufferCreateInfo&) const = 0;
		virtual RHIBufferView CreateBufferView(RHIDevice&, const RHIBufferViewCreateInfo&) const = 0;
		virtual RHIImage CreateImage(RHIDevice&, const RHIImageCreateInfo&) const = 0;
		virtual RHIImageView CreateImageView(RHIDevice&, const RHIImageViewCreateInfo&) const = 0;
		virtual RHIPipeline CreateGraphicsPipeline(RHIDevice&, const RHIGraphicsPipelineCreateInfo&) const = 0;

		// destroy
		virtual void Destroy(RHIDevice&) const = 0;
		virtual void Destroy(RHISwapchain&) const = 0;
		virtual void Destroy(RHICommandPool&) const = 0;
		virtual void Destroy(RHIBuffer&) const = 0;
		virtual void Destroy(RHIBufferView&) const = 0;
		virtual void Destroy(RHIImage&) const = 0;
		virtual void Destroy(RHIImageView&) const = 0;
		virtual void Destroy(RHIPipeline&) const = 0;

		// get
		virtual RHIQueue GetQueue(RHIDevice&, uint32_t queueIndex) const = 0;
		virtual std::vector<RHICommandBuffer> AllocateCommandBuffers(RHIDevice&, const RHICommandBufferAllocateInfo&) const = 0;
		virtual std::vector<RHIPhysicalDeviceInfo> EnumeratePhysicalDevice() const = 0;

		// command

	};

}