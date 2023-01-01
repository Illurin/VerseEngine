#pragma once

#include "rhi_base.hpp"

namespace engine {

	class RHIInstance {
	public:
		virtual void Init(const RHIInstanceInitInfo&) = 0;
		virtual void Destroy() = 0;

		// create
		virtual RHIDevice* CreateDevice(const RHIDeviceCreateInfo&) const = 0;
		virtual RHISwapchain* CreateSwapchain(const RHIDevice*, const RHISwapchainCreateInfo&) const = 0;
		virtual RHICommandPool* CreateCommandPool(const RHIDevice*, const RHICommandPoolCreateInfo&) const = 0;
		virtual RHIBuffer* CreateBuffer(const RHIDevice*, const RHIBufferCreateInfo&) const = 0;
		virtual RHIBufferView* CreateBufferView() const = 0;
		virtual RHIImage* CreateImage() const = 0;
		virtual RHIImageView* CreateImageView() const = 0;

		// destroy
		virtual void DestroyDevice(RHIDevice*) const = 0;
		virtual void DestroySwapchain(const RHIDevice*, RHISwapchain*) const = 0;
		virtual void DestroyCommandPool(const RHIDevice*, RHICommandPool*) const = 0;
		virtual void DestroyBuffer(const RHIDevice*, RHIBuffer*) const = 0;
		virtual void DestroyBufferView(const RHIDevice*, RHIBufferView*) const = 0;
		virtual void DestroyImage(const RHIDevice*, RHIImage*) const = 0;
		virtual void DestroyImageView(const RHIDevice*, RHIImageView*) const = 0;

		// get
		virtual RHIQueue* GetQueue(const RHIDevice*, uint32_t queueIndex) const = 0;
		virtual std::vector<RHICommandBuffer*> AllocateCommandBuffers(const RHIDevice*, const RHICommandBufferAllocateInfo&) const = 0;
		virtual std::vector<RHIPhysicalDeviceInfo> EnumeratePhysicalDevice() const = 0;

		// command

	};

}