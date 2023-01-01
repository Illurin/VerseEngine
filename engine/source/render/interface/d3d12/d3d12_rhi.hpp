#pragma once

#ifdef _WIN32

#include "../rhi.hpp"
#include "d3d12_base.hpp"

namespace engine {

	class D3D12Instance final : public RHIInstance {
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

	private:
		ComPtr<IDXGIFactory7> factory{ nullptr };

#ifdef _DEBUG
		ComPtr<ID3D12Debug> debugMessenger{ nullptr };
#endif // _DEBUG

	};

};

#endif // _WIN32