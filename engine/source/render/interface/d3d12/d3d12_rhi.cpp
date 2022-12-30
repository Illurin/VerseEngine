#ifdef _WIN32

#include "d3d12_rhi.hpp"

namespace engine {

	void D3D12Instance::Init(const RHIInstanceInitInfo& info) {
		uint32_t dxgiFactoryFlag{ 0 };

#ifdef _DEBUG
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugMessenger)))) {
			throw std::runtime_error("Create debug messenger failed");
		}
		debugMessenger->EnableDebugLayer();
		dxgiFactoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG
		
		if (FAILED(CreateDXGIFactory2(dxgiFactoryFlag, IID_PPV_ARGS(&factory)))) {
			throw std::runtime_error("Create DXGI factory failed");
		}
	}

	void D3D12Instance::Destroy() {
#ifdef _DEBUG
		debugMessenger->Release();
#endif // _DEBUG

		factory->Release();
	}

	RHIDevice* D3D12Instance::CreateDevice(const RHIDeviceCreateInfo& info) const {
		ComPtr<IDXGIAdapter1> adapter;
		if (FAILED(factory->EnumAdapters1(info.physicalDeviceID, &adapter))) {
			throw std::runtime_error("Cannot find chosen physical device");
		}

		ComPtr<ID3D12Device> device;
		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)))) {
			throw std::runtime_error("Create device failed");
		}
		
		auto deviceWrapper = new RHIDevice();
		static_cast<D3D12WrapperDevice*>(deviceWrapper)->SetDevice(device);
		return deviceWrapper;
	}

	RHISwapchain* D3D12Instance::CreateSwapchain(const RHIDevice* device, const RHISwapchainCreateInfo& info) const {
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
		swapchainDesc.BufferCount = info.frameCount;
		swapchainDesc.Width = info.imageExtent.width;
		swapchainDesc.Height = info.imageExtent.height;
		swapchainDesc.Format = D3D12EnumFormat(info.format).Get();
		swapchainDesc.BufferUsage = DXGI_USAGE_RENDER_TARGET_OUTPUT;
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapchainOld;
		ComPtr<IDXGISwapChain3> swapchain;

		if (FAILED(factory->CreateSwapChainForHwnd(commandQueue, info.platformInfo.hWnd, &swapchainDesc, nullptr, nullptr, &swapchainOld))) {
			throw std::runtime_error("Create swapchain failed");
		}

		if (FAILED(swapchainOld->QueryInterface(IID_PPV_ARGS(&swapchain)))) {
			throw std::runtime_error("Update swapchain failed");
		}
	}

	void D3D12Instance::DestroyDevice(RHIDevice* device) const {
		static_cast<D3D12WrapperDevice*>(device)->GetDevice()->Release();
		delete device;
	}

	std::vector<RHIPhysicalDeviceInfo> D3D12Instance::EnumeratePhysicalDevice() const {
		std::vector<RHIPhysicalDeviceInfo> infos;
		ComPtr<IDXGIAdapter1> adapter;

		for (size_t i = 1; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); i++) {
			DXGI_ADAPTER_DESC1 desc;
			adapter->GetDesc1(&desc);

			char description[128];
			sprintf_s(description, "%ws", desc.Description);

			auto info = RHIPhysicalDeviceInfo()
				.SetDeviceID(desc.DeviceId)
				.SetDeviceName(std::string(description).c_str())
				.SetApiVersion(12);

			infos.push_back(info);
		}
		return infos;
	}

}

#endif // _WIN32