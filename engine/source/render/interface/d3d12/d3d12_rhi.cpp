#ifdef _WIN32

#include "d3d12_rhi.hpp"

namespace engine {

	void D3D12Instance::Init(const RHIInstanceInitInfo& info) {
		uint32_t factoryFlag{ 0 };

#ifdef _DEBUG
		if (FAILED(D3D12GetDebugInterface(IID_PPV_ARGS(&debugMessenger)))) {
			throw std::runtime_error("Create debug messenger failed");
		}
		debugMessenger->EnableDebugLayer();
		factoryFlag |= DXGI_CREATE_FACTORY_DEBUG;
#endif // _DEBUG
		
		if (FAILED(CreateDXGIFactory2(factoryFlag, IID_PPV_ARGS(&factory)))) {
			throw std::runtime_error("Create DXGI factory failed");
		}
	}

	void D3D12Instance::Destroy() {
#ifdef _DEBUG
		debugMessenger.Reset();
#endif // _DEBUG

		factory.Reset();
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
		
		std::vector<RHIQueue*> queues;
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			auto queueWrapper = new RHIQueue();

			D3D12_COMMAND_QUEUE_DESC queueDesc;
			queueDesc.Type = D3D12EnumQueueType(info.pQueueCreateInfo[i].queueType).Get();

			ComPtr<ID3D12CommandQueue> queue;
			if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)))) {
				throw std::runtime_error("Create command queue failed");
			}
			
			static_cast<D3D12WrapperQueue*>(queueWrapper)->SetQueue(queue).SetQueueType(D3D12EnumQueueType(info.pQueueCreateInfo[i].queueType).Get());
			queues.push_back(queueWrapper);
		}

		auto deviceWrapper = new RHIDevice();
		static_cast<D3D12WrapperDevice*>(deviceWrapper)->SetDevice(device).SetQueues(queues);
		return deviceWrapper;
	}

	RHISwapchain* D3D12Instance::CreateSwapchain(const RHIDevice* device, const RHISwapchainCreateInfo& info) const {
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
		swapchainDesc.BufferCount = info.frameCount;
		swapchainDesc.Width = info.imageExtent.width;
		swapchainDesc.Height = info.imageExtent.height;
		swapchainDesc.Format = D3D12EnumFormat(info.format).Get();
		swapchainDesc.AlphaMode = D3D12EnumAlphaMode(info.alphaMode).Get();
		swapchainDesc.BufferUsage = D3D12EnumImageUsage(info.imageUsage).Get();
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.SampleDesc.Count = 1;

		ComPtr<IDXGISwapChain1> swapchainOld;
		ComPtr<IDXGISwapChain3> swapchain;

		if (FAILED(factory->CreateSwapChainForHwnd(static_cast<const D3D12WrapperQueue*>(info.queue)->GetQueue().Get(), info.platformInfo.hWnd, &swapchainDesc, nullptr, nullptr, &swapchainOld))) {
			throw std::runtime_error("Create swapchain failed");
		}

		if (FAILED(swapchainOld->QueryInterface(IID_PPV_ARGS(&swapchain)))) {
			throw std::runtime_error("Update swapchain failed");
		}

		auto swapchainWrapper = new RHISwapchain();
		static_cast<D3D12WrapperSwapchain*>(swapchainWrapper)->SetSwapchain(swapchain);
		return swapchainWrapper;
	}

	RHICommandPool* D3D12Instance::CreateCommandPool(const RHIDevice* device, const RHICommandPoolCreateInfo& info) const {
		ComPtr<ID3D12CommandAllocator> cmdAllocator;
		if (FAILED(static_cast<const D3D12WrapperDevice*>(device)->GetDevice()->CreateCommandAllocator(static_cast<const D3D12WrapperQueue*>(info.queue)->GetQueueType(), IID_PPV_ARGS(&cmdAllocator)))) {
			throw std::runtime_error("Create command allocator failed");
		}

		auto cmdPoolWrapper = new RHICommandPool();
		static_cast<D3D12WrapperCommandPool*>(cmdPoolWrapper)->SetQueueType(static_cast<const D3D12WrapperQueue*>(info.queue)->GetQueueType()).SetCommandAllocator(cmdAllocator);
		return cmdPoolWrapper;
	}

	void D3D12Instance::DestroyDevice(RHIDevice* device) const {
		for (auto queue : static_cast<D3D12WrapperDevice*>(device)->GetQueues()) {
			static_cast<D3D12WrapperQueue*>(queue)->GetQueue().Reset();
			delete queue;
		}
		static_cast<D3D12WrapperDevice*>(device)->GetDevice().Reset();
		delete device;
	}

	void D3D12Instance::DestroySwapchain(const RHIDevice*, RHISwapchain* swapchain) const {
		static_cast<D3D12WrapperSwapchain*>(swapchain)->GetSwapchain().Reset();
		delete swapchain;
	}

	void D3D12Instance::DestroyCommandPool(const RHIDevice* device, RHICommandPool* cmdPool) const {
		static_cast<D3D12WrapperCommandPool*>(cmdPool)->GetCommandAllocator().Reset();
		for (auto& cmdBuffer : static_cast<D3D12WrapperCommandPool*>(cmdPool)->GetCommandBuffers()) {
			static_cast<D3D12WrapperCommandBuffer*>(cmdBuffer)->GetGraphicsCommandList().Reset();
			delete cmdBuffer;
		}
		delete cmdPool;
	}

	RHIQueue* D3D12Instance::GetQueue(const RHIDevice* device, uint32_t queueIndex) const {
		auto queues = static_cast<const D3D12WrapperDevice*>(device)->GetQueues();
		if (queueIndex >= queues.size()) {
			throw std::runtime_error("Queue index out of range");
		}
		return queues[queueIndex];
	}

	std::vector<RHICommandBuffer*> D3D12Instance::AllocateCommandBuffers(const RHIDevice* device, const RHICommandBufferAllocateInfo& info) const {
		std::vector<RHICommandBuffer*> cmdWrappers;
		for (uint32_t i = 0; i < info.commandBufferCount; i++) {
			ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;
			if (FAILED(static_cast<const D3D12WrapperDevice*>(device)->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, static_cast<D3D12WrapperCommandPool*>(info.commandPool)->GetCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&graphicsCommandList)))) {
				throw std::runtime_error("Create command lists failed");
			}

			auto cmdWrapper = new RHICommandBuffer();
			static_cast<D3D12WrapperCommandBuffer*>(cmdWrapper)->SetGraphicsCommandList(graphicsCommandList);
			cmdWrappers.push_back(cmdWrapper);
		}

		auto poolCmdBuffers = static_cast<D3D12WrapperCommandPool*>(info.commandPool)->GetCommandBuffers();
		poolCmdBuffers.insert(poolCmdBuffers.end(), cmdWrappers.begin(), cmdWrappers.end());
		static_cast<D3D12WrapperCommandPool*>(info.commandPool)->SetCommandBuffers(poolCmdBuffers);

		return cmdWrappers;
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