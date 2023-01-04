#include "d3d12_rhi.hpp"

#ifdef _DIRECT3D12

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

	RHIDevice D3D12Instance::CreateDevice(const RHIDeviceCreateInfo& info) const {
		ComPtr<IDXGIAdapter1> adapter;
		if (FAILED(factory->EnumAdapters1(info.physicalDeviceID, &adapter))) {
			throw std::runtime_error("Cannot find chosen physical device");
		}

		ComPtr<ID3D12Device> device;
		if (FAILED(D3D12CreateDevice(adapter.Get(), D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)))) {
			throw std::runtime_error("Create device failed");
		}
		
		std::vector<RHIQueue> queues;
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			auto queueWrapper = new D3D12WrapperQueue();

			D3D12_COMMAND_QUEUE_DESC queueDesc;
			queueDesc.Type = D3D12EnumQueueType(info.pQueueCreateInfo[i].queueType).Get();

			ComPtr<ID3D12CommandQueue> queue;
			if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)))) {
				throw std::runtime_error("Create command queue failed");
			}
			
			queueWrapper->SetQueue(queue).SetQueueType(D3D12EnumQueueType(info.pQueueCreateInfo[i].queueType).Get());
			queues.push_back(queueWrapper);
		}

		auto deviceWrapper = new D3D12WrapperDevice();
		deviceWrapper->SetDevice(device).SetQueues(queues);
		return deviceWrapper;
	}

	RHISwapchain D3D12Instance::CreateSwapchain(RHIDevice& device, const RHISwapchainCreateInfo& info) const {
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc;
		swapchainDesc.BufferCount = info.frameCount;
		swapchainDesc.Width = info.imageExtent.width;
		swapchainDesc.Height = info.imageExtent.height;
		swapchainDesc.Format = D3D12EnumFormat(info.format).Get();
		swapchainDesc.AlphaMode = D3D12EnumAlphaMode(info.alphaMode).Get();
		swapchainDesc.BufferUsage = D3D12EnumUsage(info.imageUsage).Get();
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

		auto swapchainWrapper = new D3D12WrapperSwapchain();
		swapchainWrapper->SetSwapchain(swapchain);
		return swapchainWrapper;
	}

	RHICommandPool D3D12Instance::CreateCommandPool(RHIDevice& device, const RHICommandPoolCreateInfo& info) const {
		ComPtr<ID3D12CommandAllocator> cmdAllocator;
		if (FAILED(static_cast<const D3D12WrapperDevice*>(device)->GetDevice()->CreateCommandAllocator(static_cast<const D3D12WrapperQueue*>(info.queue)->GetQueueType(), IID_PPV_ARGS(&cmdAllocator)))) {
			throw std::runtime_error("Create command allocator failed");
		}
		
		auto cmdPoolWrapper = new D3D12WrapperCommandPool();
		cmdPoolWrapper->SetQueueType(static_cast<const D3D12WrapperQueue*>(info.queue)->GetQueueType()).SetCommandAllocator(cmdAllocator);
		return cmdPoolWrapper;
	}

	RHIBuffer D3D12Instance::CreateBuffer(RHIDevice& device, const RHIBufferCreateInfo& info) const {
		ComPtr<ID3D12Resource> buffer;
		if (FAILED(static_cast<D3D12WrapperDevice*>(device)->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD),
			D3D12_HEAP_FLAG_NONE,
			&CD3DX12_RESOURCE_DESC::Buffer(info.size),
			D3D12EnumResourceState(info.usage).Get(),
			nullptr,
			IID_PPV_ARGS(&buffer)))) {
			throw std::runtime_error("Create buffer failed");
		}
		
		auto bufferWrapper = new D3D12WrapperBuffer();
		bufferWrapper->SetResource(buffer);
		return bufferWrapper;
	}

	RHIImage D3D12Instance::CreateImage(RHIDevice& device, const RHIImageCreateInfo& info) const {
		D3D12_RESOURCE_DESC resourceDesc;
		resourceDesc.Format = D3D12EnumFormat(info.format).Get();
		resourceDesc.Dimension = D3D12EnumImageType(info.imageType).Get();
		resourceDesc.Width = info.extent.width;
		resourceDesc.Height = info.extent.height;
		resourceDesc.DepthOrArraySize = info.extent.depth;
		resourceDesc.Alignment = 0;
		resourceDesc.MipLevels = info.mipLevels;
		resourceDesc.Layout = D3D12_TEXTURE_LAYOUT_UNKNOWN;
		resourceDesc.SampleDesc.Count = D3D12EnumSampleCount(info.sampleCount).Get();
		static_cast<D3D12WrapperDevice*>(device)->GetDevice()->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &resourceDesc.SampleDesc.Quality, sizeof(UINT));

		ComPtr<ID3D12Resource> image;
		if (FAILED(static_cast<D3D12WrapperDevice*>(device)->GetDevice()->CreateCommittedResource(
			&CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT),
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12EnumResourceState(info.usage).Get(),
			nullptr,
			IID_PPV_ARGS(&image)))) {
			throw std::runtime_error("Create image failed");
		}

		auto imageWrapper = new D3D12WrapperImage();
		imageWrapper->SetResource(image);
		return imageWrapper;
	}

	RHIImageView D3D12Instance::CreateImageView(RHIDevice&, const RHIImageViewCreateInfo&) const {
		
	}

	RHIDescriptorPool D3D12Instance::CreateDescriptorPool(RHIDevice& device, const RHIDescriptorPoolCreateInfo& info) const {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = info.descriptorCount;
		heapDesc.Type = D3D12EnumHeapType(info.descriptorType).Get();

		ComPtr<ID3D12Heap> heap;
		static_cast<D3D12WrapperDevice*>(device)->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));

		auto descriptorPoolWrapper = new D3D12WrapperDescriptorPool();
		descriptorPoolWrapper->SetHeap(heap);
		return descriptorPoolWrapper;
	}

	void D3D12Instance::Destroy(RHIDevice& device) const {
		for (auto queue : static_cast<D3D12WrapperDevice*>(device)->GetQueues()) {
			static_cast<D3D12WrapperQueue*>(queue)->GetQueue().Reset();
			delete queue;
		}
		static_cast<D3D12WrapperDevice*>(device)->GetDevice().Reset();
		delete device;
	}

	void D3D12Instance::Destroy(RHISwapchain& swapchain) const {
		static_cast<D3D12WrapperSwapchain*>(swapchain)->GetSwapchain().Reset();
		delete swapchain;
	}

	void D3D12Instance::Destroy(RHICommandPool& cmdPool) const {
		static_cast<D3D12WrapperCommandPool*>(cmdPool)->GetCommandAllocator().Reset();
		for (auto& cmdBuffer : static_cast<D3D12WrapperCommandPool*>(cmdPool)->GetCommandBuffers()) {
			static_cast<D3D12WrapperCommandBuffer*>(cmdBuffer)->GetGraphicsCommandList().Reset();
			delete cmdBuffer;
		}
		delete cmdPool;
	}

	void D3D12Instance::Destroy(RHIBuffer& buffer) const {
		static_cast<D3D12WrapperBuffer*>(buffer)->GetResource().Reset();
		delete buffer;
	}

	void D3D12Instance::Destroy(RHIImage& image) const {
		static_cast<D3D12WrapperImage*>(image)->GetResource().Reset();
		delete image;
	}

	void D3D12Instance::Destroy(RHIDescriptorPool& descriptorPool) const {
		static_cast<D3D12WrapperDescriptorPool*>(descriptorPool)->GetHeap().Reset();
		delete descriptorPool;
	}

	RHIQueue D3D12Instance::GetQueue(RHIDevice& device, uint32_t queueIndex) const {
		auto queues = static_cast<const D3D12WrapperDevice*>(device)->GetQueues();
		if (queueIndex >= queues.size()) {
			throw std::runtime_error("Queue index out of range");
		}
		return queues[queueIndex];
	}

	std::vector<RHICommandBuffer> D3D12Instance::AllocateCommandBuffers(RHIDevice& device, const RHICommandBufferAllocateInfo& info) const {
		std::vector<RHICommandBuffer> cmdWrappers;
		for (uint32_t i = 0; i < info.commandBufferCount; i++) {
			ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;
			if (FAILED(static_cast<D3D12WrapperDevice*>(device)->GetDevice()->CreateCommandList(0, D3D12_COMMAND_LIST_TYPE_DIRECT, static_cast<D3D12WrapperCommandPool*>(info.commandPool)->GetCommandAllocator().Get(), nullptr, IID_PPV_ARGS(&graphicsCommandList)))) {
				throw std::runtime_error("Create command lists failed");
			}

			auto cmdWrapper = new D3D12WrapperCommandBuffer();
			cmdWrapper->SetGraphicsCommandList(graphicsCommandList);
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

#endif // _DIRECT3D12