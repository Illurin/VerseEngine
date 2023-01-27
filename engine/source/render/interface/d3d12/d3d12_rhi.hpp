#pragma once

#include "render/interface/rhi.hpp"
#include "d3d12_base.hpp"

#ifdef _DIRECT3D12

namespace engine {

	class D3D12WrapperInstance final : public RHIInstance_T {
	public:
		void Init(const RHIInstanceInitInfo&) override;
		void Destroy() override;

		RHIDevice CreateDevice(const RHIDeviceCreateInfo&) const override;

		std::vector<RHIPhysicalDeviceInfo> GetPhysicalDeviceInfo() const override;

	private:
		IDXGIFactory7* factory{ nullptr };

#ifdef _DEBUG
		ID3D12Debug* debugMessenger{ nullptr };
#endif // _DEBUG
	};

	

}

#endif // _DIRECT3D12