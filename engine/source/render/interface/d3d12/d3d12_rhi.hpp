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

		// destroy
		void DestroyDevice(RHIDevice*) const override;

		// get
		std::vector<RHIPhysicalDeviceInfo> EnumeratePhysicalDevice() const override;

	private:
		ComPtr<IDXGIFactory7> factory{ nullptr };
		

#ifdef _DEBUG
		ComPtr<ID3D12Debug> debugMessenger{ nullptr };
#endif // _DEBUG

	};

};

#endif // _WIN32