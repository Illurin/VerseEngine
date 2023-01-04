#pragma once

#include "../rhi.hpp"
#include "d3d12_base.hpp"

#ifdef _DIRECT3D12

namespace engine {

	class D3D12Instance final : public RHIInstance {
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
		RHIImageView CreateImageView(RHIDevice&, const RHIImageViewCreateInfo&) const;
		RHIDescriptorPool CreateDescriptorPool(RHIDevice&, const RHIDescriptorPoolCreateInfo&) const override;

		// destroy
		void Destroy(RHIDevice&) const override;
		void Destroy(RHISwapchain&) const override;
		void Destroy(RHICommandPool&) const override;
		void Destroy(RHIBuffer&) const override;
		void Destroy(RHIBufferView&) const override;
		void Destroy(RHIImage&) const override;
		void Destroy(RHIImageView&) const override;
		void Destroy(RHIDescriptorPool&) const override;

		// get
		RHIQueue GetQueue(RHIDevice&, uint32_t queueIndex) const override;
		std::vector<RHICommandBuffer> AllocateCommandBuffers(RHIDevice&, const RHICommandBufferAllocateInfo&) const override;
		std::vector<RHIPhysicalDeviceInfo> EnumeratePhysicalDevice() const override;

	private:
		ComPtr<IDXGIFactory7> factory{ nullptr };

#ifdef _DEBUG
		ComPtr<ID3D12Debug> debugMessenger{ nullptr };
#endif // _DEBUG

	};

};

#endif // _DIRECT3D12