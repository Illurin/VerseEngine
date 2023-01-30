#include "d3d12_rhi.hpp"

#ifdef _DIRECT3D12

namespace engine {

	void D3D12WrapperInstance::Init(const rhi::InstanceCreateInfo& info) {
		uint32_t factoryFlag = 0;

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

		this->info.apiName = info.pApiName;
		this->info.applicationName = info.pApplicationName;
		this->info.applicationVersion = info.applicationVersion;
	}

	std::vector<rhi::PhysicalDeviceInfo> D3D12WrapperInstance::GetPhysicalDeviceInfo() const {
		std::vector<rhi::PhysicalDeviceInfo> infos;
		IDXGIAdapter1* adapter;

		for (UINT i = 0; DXGI_ERROR_NOT_FOUND != factory->EnumAdapters1(i, &adapter); i++) {
			DXGI_ADAPTER_DESC desc;
			adapter->GetDesc(&desc);
			adapter->Release();
			
			auto info = rhi::PhysicalDeviceInfo()
				.SetId(static_cast<uint32_t>(i))
				.SetVendorId(desc.VendorId)
				.SetDeviceId(desc.DeviceId)
				.SetDeviceDescription(WStringToString(desc.Description));

			infos.emplace_back(info);
		}
		return infos;
	}

	rhi::Device D3D12WrapperInstance::CreateDevice(const rhi::DeviceCreateInfo& info) const {
		IDXGIAdapter1* adapter;
		if (FAILED(factory->EnumAdapters1(info.physicalDeviceId, &adapter))) {
			throw std::runtime_error("Cannot find chosen physical device");
		}

		ID3D12Device2* device;
		if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)))) {
			throw std::runtime_error("Create device failed");
		}
		adapter->Release();
		
		std::vector<rhi::Queue> queues(info.queueCreateInfoCount);
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12EnumCommandListType(info.pQueueCreateInfos[i].queueType).Get();

			ID3D12CommandQueue* queue;
			if (FAILED(device->CreateCommandQueue(&queueDesc, IID_PPV_ARGS(&queue)))) {
				throw std::runtime_error("Create command queue failed");
			}
			
			auto queueWrapper = new D3D12WrapperQueue();
			queueWrapper->queue = queue;
			queueWrapper->commandListType = D3D12EnumCommandListType(info.pQueueCreateInfos[i].queueType).Get();
			queues[i] = queueWrapper;
		}

		auto deviceWrapper = new D3D12WrapperDevice();
		deviceWrapper->factory = factory;
		deviceWrapper->device = device;
		deviceWrapper->queues = queues;
		return deviceWrapper;
	}

	ShaderCompiler D3D12WrapperInstance::CreateShaderCompiler() const {
		ShaderCompiler shaderCompiler;
		shaderCompiler.AddArgument(ShaderCompileArgument::matrix_column_major);
		shaderCompiler.AddArgument(ShaderCompileArgument::enable_debug_info);
		shaderCompiler.AddArgument(ShaderCompileArgument::embed_debug);
		shaderCompiler.AddArgument(ShaderCompileArgument::treat_warnings_as_errors);
		shaderCompiler.AddIncludePath(StringToWString(GlobalConfig::configPath).c_str());
		return shaderCompiler;
	}



	rhi::Queue D3D12WrapperDevice::GetQueue(uint32_t queueIndex) const {
		if (queueIndex >= queues.size()) {
			throw std::runtime_error("Queue index out of range");
		}
		return queues[queueIndex];
	}

	rhi::Swapchain D3D12WrapperDevice::CreateSwapchain(const rhi::SwapchainCreateInfo& info) const {
		DXGI_SWAP_CHAIN_DESC1 swapchainDesc = {};
		swapchainDesc.BufferCount = info.frameCount;
		swapchainDesc.Width = info.imageExtent.width;
		swapchainDesc.Height = info.imageExtent.height;
		swapchainDesc.Format = D3D12EnumFormat(info.format).Get();
		swapchainDesc.AlphaMode = D3D12EnumAlphaMode(info.alphaMode).Get();
		swapchainDesc.BufferUsage = D3D12EnumUsage(info.imageUsage).Get();
		swapchainDesc.SwapEffect = DXGI_SWAP_EFFECT_FLIP_DISCARD;
		swapchainDesc.SampleDesc.Count = 1;
		swapchainDesc.SampleDesc.Quality = 0;
		
		IDXGISwapChain1* swapchainOld;
		IDXGISwapChain3* swapchain;

		if (FAILED(factory->CreateSwapChainForHwnd(static_cast<D3D12WrapperQueue*>(info.queue)->queue, info.platformInfo.hWnd, &swapchainDesc, nullptr, nullptr, &swapchainOld))) {
			throw std::runtime_error("Create swapchain failed");
		}

		if (FAILED(swapchainOld->QueryInterface(IID_PPV_ARGS(&swapchain)))) {
			throw std::runtime_error("Update swapchain failed");
		}

		swapchain->GetDesc1(&swapchainDesc);

		std::vector<rhi::Image> swapchainImages(info.frameCount);
		for (uint32_t i = 0; i < info.frameCount; i++) {
			ID3D12Resource* resource;
			swapchain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&resource));
			
			auto imageWrapper = new D3D12WrapperImage();
			imageWrapper->resource = resource;
			swapchainImages[i] = imageWrapper;
		}

		auto swapchainWrapper = new D3D12WrapperSwapchain();
		swapchainWrapper->swapchain = swapchain;
		swapchainWrapper->images = swapchainImages;
		swapchainWrapper->imageExtent = rhi::Extent2D().SetWidth(swapchainDesc.Width).SetHeight(swapchainDesc.Height);
		return swapchainWrapper;
	}

	rhi::CommandPool D3D12WrapperDevice::CreateCommandPool(const rhi::CommandPoolCreateInfo& info) const {
		auto commandListType = static_cast<D3D12WrapperQueue*>(info.queue)->commandListType;
		ID3D12CommandAllocator* cmdAllocator;
		if (FAILED(device->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&cmdAllocator)))) {
			throw std::runtime_error("Create command allocator failed");
		}
		
		auto cmdPoolWrapper = new D3D12WrapperCommandPool();
		cmdPoolWrapper->commandAllocator = cmdAllocator;
		cmdPoolWrapper->commandListType = commandListType;
		return cmdPoolWrapper;
	}

	rhi::Buffer D3D12WrapperDevice::CreateBuffer(const rhi::BufferCreateInfo& info) const {
		/*ComPtr<ID3D12Resource> buffer;
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
		return bufferWrapper;*/
		return new D3D12WrapperBuffer();
	}

	rhi::Image D3D12WrapperDevice::CreateImage(const rhi::ImageCreateInfo& info) const {
		/*D3D12_RESOURCE_DESC resourceDesc;
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
		return imageWrapper;*/
		return new D3D12WrapperImage();
	}

	/*rhi::DescriptorPool D3D12WrapperDevice::CreateDescriptorPool(rhi::Device& device, const rhi::DescriptorPoolCreateInfo& info) const {
		D3D12_DESCRIPTOR_HEAP_DESC heapDesc = {};
		heapDesc.NumDescriptors = info.descriptorCount;
		heapDesc.Type = D3D12EnumHeapType(info.descriptorType).Get();

		ComPtr<ID3D12Heap> heap;
		static_cast<D3D12WrapperDevice*>(device)->GetDevice()->CreateDescriptorHeap(&heapDesc, IID_PPV_ARGS(&heap));

		auto descriptorPoolWrapper = new D3D12WrapperDescriptorPool();
		descriptorPoolWrapper->SetHeap(heap);
		return descriptorPoolWrapper;
	}*/

	rhi::RenderPass D3D12WrapperDevice::CreateRenderPass(const rhi::RenderPassCreateInfo& info) const {
		auto renderPass = new D3D12WrapperRenderPass();
		renderPass->colorAttachmentCount = info.colorAttachmentCount;
		renderPass->pColorAttachments = info.pColorAttachments;
		renderPass->pDepthStencilAttachment = info.pDepthStencilAttachment;
		return renderPass;
	}

	rhi::ShaderModule D3D12WrapperDevice::CreateShaderModule(const rhi::ShaderModuleCreateInfo& info) const {
		std::vector<char> binarySource(info.sourceCodeSize);
		std::copy(info.pSourceCode, info.pSourceCode + info.sourceCodeSize, binarySource.begin());
		auto shaderModule = new D3D12WrapperShaderModule();
		shaderModule->binarySource = binarySource;
		shaderModule->shaderStage = info.shaderStage;
		return shaderModule;
	}

	rhi::Pipeline D3D12WrapperDevice::CreateGraphicsPipeline(const rhi::GraphicsPipelineCreateInfo& info) const {
		D3D12_RASTERIZER_DESC rasterizationDesc = {};
		rasterizationDesc.AntialiasedLineEnable = info.rasterizationInfo.smoothLine;
		rasterizationDesc.CullMode = D3D12EnumCullMode(info.rasterizationInfo.cullMode).Get();
		rasterizationDesc.FillMode = D3D12EnumFillMode(info.rasterizationInfo.polygonMode).Get();
		rasterizationDesc.DepthBias = static_cast<INT>(info.rasterizationInfo.depthBias);
		rasterizationDesc.DepthBiasClamp = info.rasterizationInfo.depthClamp;
		rasterizationDesc.SlopeScaledDepthBias = info.rasterizationInfo.slopeScaledDepthBias;
		rasterizationDesc.ForcedSampleCount = D3D12EnumSampleCount(info.multisampleInfo.rasterizationSamples).Get();
		rasterizationDesc.MultisampleEnable = info.multisampleInfo.rasterizationSamples == rhi::SampleCount::Count1 ? false : true;
		rasterizationDesc.FrontCounterClockwise = D3D12EnumFrontCounterClockwise(info.rasterizationInfo.frontFace).Get();
		
		D3D12_DEPTH_STENCILOP_DESC frontOpDesc = {};
		frontOpDesc.StencilFunc = D3D12EnumComparisonFunc(info.depthStencilInfo.frontOp.compareOp).Get();
		frontOpDesc.StencilPassOp = D3D12EnumStencilOp(info.depthStencilInfo.frontOp.passOp).Get();
		frontOpDesc.StencilFailOp = D3D12EnumStencilOp(info.depthStencilInfo.frontOp.failOp).Get();
		frontOpDesc.StencilDepthFailOp = D3D12EnumStencilOp(info.depthStencilInfo.frontOp.depthFailOp).Get();

		D3D12_DEPTH_STENCILOP_DESC backOpDesc = {};
		backOpDesc.StencilFunc = D3D12EnumComparisonFunc(info.depthStencilInfo.backOp.compareOp).Get();
		backOpDesc.StencilPassOp = D3D12EnumStencilOp(info.depthStencilInfo.backOp.passOp).Get();
		backOpDesc.StencilFailOp = D3D12EnumStencilOp(info.depthStencilInfo.backOp.failOp).Get();
		backOpDesc.StencilDepthFailOp = D3D12EnumStencilOp(info.depthStencilInfo.backOp.depthFailOp).Get();

		D3D12_DEPTH_STENCIL_DESC1 depthStencilDesc = {};
		depthStencilDesc.DepthEnable = info.depthStencilInfo.depthTestEnable;
		depthStencilDesc.DepthWriteMask = info.depthStencilInfo.depthWriteEnable ? D3D12_DEPTH_WRITE_MASK_ALL : D3D12_DEPTH_WRITE_MASK_ZERO;
		depthStencilDesc.DepthFunc = D3D12EnumComparisonFunc(info.depthStencilInfo.depthCompareOp).Get();
		depthStencilDesc.DepthBoundsTestEnable = info.depthStencilInfo.depthBoundsTestEnable;
		depthStencilDesc.StencilEnable = info.depthStencilInfo.stencilTestEnable;
		depthStencilDesc.StencilReadMask = info.depthStencilInfo.stencilCompareMask;
		depthStencilDesc.StencilWriteMask = info.depthStencilInfo.stencilWriteMask;
		depthStencilDesc.FrontFace = frontOpDesc;
		depthStencilDesc.BackFace = backOpDesc;
		
		D3D12_BLEND_DESC blendDesc = {};
		blendDesc.IndependentBlendEnable = true;
		blendDesc.AlphaToCoverageEnable = info.multisampleInfo.alphaToCoverageEnable;
		
		for (uint32_t i = 0; i < info.colorBlendInfo.attachmentCount; i++) {
			auto& attachment = info.colorBlendInfo.pAttachments[i];
			D3D12_RENDER_TARGET_BLEND_DESC renderTargetBlendDesc = {};
			renderTargetBlendDesc.BlendEnable = attachment.blendEnable;
			renderTargetBlendDesc.RenderTargetWriteMask = D3D12EnumColorWriteEnable(attachment.colorWriteMask).Get();
			renderTargetBlendDesc.BlendOp = D3D12EnumBlendOp(attachment.colorBlendOp).Get();
			renderTargetBlendDesc.BlendOpAlpha = D3D12EnumBlendOp(attachment.alphaBlendOp).Get();
			renderTargetBlendDesc.SrcBlend = D3D12EnumBlend(attachment.srcColorBlendFactor).Get();
			renderTargetBlendDesc.SrcBlendAlpha = D3D12EnumBlend(attachment.srcAlphaBlendFactor).Get();
			renderTargetBlendDesc.DestBlend = D3D12EnumBlend(attachment.dstColorBlendFactor).Get();
			renderTargetBlendDesc.DestBlendAlpha = D3D12EnumBlend(attachment.dstAlphaBlendFactor).Get();
			blendDesc.RenderTarget[i] = renderTargetBlendDesc;
		}

		D3D12StreamingGraphicsPipelineDesc pipelineDesc = {};
		pipelineDesc.rasterizationDesc.value = rasterizationDesc;
		pipelineDesc.depthStencilDesc.value = depthStencilDesc;
		pipelineDesc.blendDesc.value = blendDesc;
		pipelineDesc.primitiveTopologyType.value = D3D12EnumPrimitiveTopologyType(info.inputAssemblyInfo.topology).Get();
		pipelineDesc.ibStripCutValue.value = info.inputAssemblyInfo.primitivieRestart ?
			D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF :
			D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineDesc.sampleDesc.value.Count = D3D12EnumSampleCount(info.multisampleInfo.rasterizationSamples).Get();
		pipelineDesc.sampleDesc.value.Quality = 0xffffffff;

		auto renderPass = static_cast<D3D12WrapperRenderPass*>(info.renderPass);
		pipelineDesc.renderTargetFormats.value.NumRenderTargets = static_cast<UINT>(renderPass->colorAttachmentCount);
		for (uint32_t i = 0; i < renderPass->colorAttachmentCount; i++) {
			pipelineDesc.renderTargetFormats.value.RTFormats[i] = D3D12EnumFormat(renderPass->pColorAttachments[i].format).Get();
		}
		if (renderPass->pDepthStencilAttachment) {
			pipelineDesc.depthStencilFormat.value = D3D12EnumFormat(renderPass->pDepthStencilAttachment->format).Get();
		}

		for (uint32_t i = 0; i < info.shaderStageInfo.shaderModuleCount; i++) {
			auto shaderModule = static_cast<D3D12WrapperShaderModule*>(info.shaderStageInfo.pShaderModules[i]);
			D3D12_SHADER_BYTECODE shaderByteCode = {};
			shaderByteCode.pShaderBytecode = shaderModule->binarySource.data();
			shaderByteCode.BytecodeLength = shaderModule->binarySource.size();
			switch (shaderModule->shaderStage) {
			case rhi::ShaderStage::Vertex:
				pipelineDesc.vs.value = shaderByteCode;
				break;
			case rhi::ShaderStage::TessellationControl:
				pipelineDesc.hs.value = shaderByteCode;
				break;
			case rhi::ShaderStage::TessellationEvaluation:
				pipelineDesc.ds.value = shaderByteCode;
				break;
			case rhi::ShaderStage::Geometry:
				pipelineDesc.gs.value = shaderByteCode;
				break;
			case rhi::ShaderStage::Fragment:
				pipelineDesc.ps.value = shaderByteCode;
				break;
			}
		}
		
		ID3DBlob* rootSignatureBlob;
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_0, &rootSignatureBlob, nullptr);

		ID3D12RootSignature* rootSignature;
		device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature));
		pipelineDesc.rootSignature.value = rootSignature;

		D3D12_PIPELINE_STATE_STREAM_DESC streamDesc = {};
		streamDesc.SizeInBytes = sizeof(pipelineDesc);
		streamDesc.pPipelineStateSubobjectStream = &pipelineDesc;

		ID3D12PipelineState* pipeline;
		if (FAILED(device->CreatePipelineState(&streamDesc, IID_PPV_ARGS(&pipeline)))) {
			throw std::runtime_error("Create graphics pipeline failed");
		}

		std::vector<D3D12_RECT> scissors(info.viewportInfo.scissorCount);
		for (uint32_t i = 0; i < info.viewportInfo.scissorCount; i++) {
			scissors[i].left = static_cast<UINT>(info.viewportInfo.pScissors[i].offset.x);
			scissors[i].top = static_cast<UINT>(info.viewportInfo.pScissors[i].offset.y);
			scissors[i].right = static_cast<UINT>(info.viewportInfo.pScissors[i].extent.width);
			scissors[i].bottom = static_cast<UINT>(info.viewportInfo.pScissors[i].extent.height);
		}

		std::vector<D3D12_VIEWPORT> viewports(info.viewportInfo.viewportCount);
		for (uint32_t i = 0; i < info.viewportInfo.viewportCount; i++) {
			viewports[i].TopLeftX = info.viewportInfo.pViewports[i].x;
			viewports[i].TopLeftY = info.viewportInfo.pViewports[i].y;
			viewports[i].Width = info.viewportInfo.pViewports[i].width;
			viewports[i].Height = info.viewportInfo.pViewports[i].height;
			viewports[i].MinDepth = info.viewportInfo.pViewports[i].minDepth;
			viewports[i].MaxDepth = info.viewportInfo.pViewports[i].maxDepth;
		}

		auto pipelineWrapper = new D3D12WrapperPipeline();
		pipelineWrapper->pipeline = pipeline;
		pipelineWrapper->scissors = scissors;
		pipelineWrapper->viewports = viewports;
		pipelineWrapper->depthStencilInfo.minDepthBounds = info.depthStencilInfo.maxDepthBounds;
		pipelineWrapper->depthStencilInfo.maxDepthBounds = info.depthStencilInfo.maxDepthBounds;
		return pipelineWrapper;
	}

	rhi::Framebuffer D3D12WrapperDevice::CreateFramebuffer(const rhi::FramebufferCreateInfo&) const {

		return new D3D12WrapperFramebuffer();
	}

	rhi::Fence D3D12WrapperDevice::CreateFence(const rhi::FenceCreateInfo&) const {

		return new D3D12WrapperFence();
	}



	void D3D12WrapperQueue::SubmitCommandBuffers(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) {

	}

	void D3D12WrapperQueue::PresentSwapchain(rhi::Swapchain swapchain, uint32_t imageIndex) {

	}



	rhi::Extent2D D3D12WrapperSwapchain::GetImageExtent() const {
		return imageExtent;
	}

	std::vector<rhi::Image> D3D12WrapperSwapchain::GetImages() const {
		return images;
	}

	uint32_t D3D12WrapperSwapchain::AcquireNextImage(rhi::Fence) {

		return 0;
	}



	void D3D12WrapperCommandPool::Reset() {

	}

	std::vector<rhi::CommandBuffer> D3D12WrapperCommandPool::AllocateCommandBuffers(uint32_t bufferCount) {
		/*std::vector<rhi::CommandBuffer> cmdWrappers;
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

		return cmdWrappers;*/
		return std::vector<rhi::CommandBuffer>();
	}



	void D3D12WrapperCommandBuffer::Reset() {

	}

	void D3D12WrapperCommandBuffer::Begin(const rhi::CommandBufferBeginInfo&) {

	}

	void D3D12WrapperCommandBuffer::End() {

	}

	void D3D12WrapperCommandBuffer::BeginRenderPass(const rhi::RenderPassBeginInfo&) {

	}

	void D3D12WrapperCommandBuffer::EndRenderPass() {

	}

	void D3D12WrapperCommandBuffer::BindPipeline(const rhi::Pipeline&) {

	}

	void D3D12WrapperCommandBuffer::BindVertexBuffer(uint32_t bindingCount, rhi::Buffer* pBuffer, uint64_t* pOffsets) {

	}

	void D3D12WrapperCommandBuffer::BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) {

	}

	void D3D12WrapperCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const {

	}

	void D3D12WrapperCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstVertex, int32_t vertexOffset, uint32_t firstInstance) const {

	}



	void D3D12WrapperFence::Reset() {

	}

	void D3D12WrapperFence::Wait() {

	}

	bool D3D12WrapperFence::GetCurrentStatus() const {

		return false;
	}



	void D3D12WrapperInstance::Destroy() {
#ifdef _DEBUG
		debugMessenger->Release();
#endif // _DEBUG
		factory->Release();
		delete this;
	}

	void D3D12WrapperDevice::Destroy() {
		for (auto& queue : queues) {
			static_cast<D3D12WrapperQueue*>(queue)->queue->Release();
			delete queue;
		}
		device->Release();
		delete this;
	}

	void D3D12WrapperSwapchain::Destroy() {
		for (auto& image : images) delete image;
		swapchain->Release();
		delete this;
	}

	void D3D12WrapperCommandPool::Destroy() {
		commandAllocator->Release();
		delete this;
	}

	void D3D12WrapperBuffer::Destroy() {

	}

	void D3D12WrapperImage::Destroy() {

	}

	void D3D12WrapperRenderPass::Destroy() {
		delete this;
	}

	void D3D12WrapperShaderModule::Destroy() {
		delete this;
	}

	void D3D12WrapperPipeline::Destroy() {
		pipeline->Release();
		delete this;
	}

	void D3D12WrapperFramebuffer::Destroy() {

	}

	void D3D12WrapperFence::Destroy() {

	}

}

#endif // _DIRECT3D12