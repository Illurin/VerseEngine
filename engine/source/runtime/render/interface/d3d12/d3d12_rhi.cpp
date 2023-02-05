#include "d3d12_rhi.hpp"

#ifdef _DIRECT3D12

namespace engine {


	/* -------------------- D3D12WrapperInstance -------------------- */


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

		decltype(D3D12WrapperDevice::device) device;
		if (FAILED(D3D12CreateDevice(adapter, D3D_FEATURE_LEVEL_12_1, IID_PPV_ARGS(&device)))) {
			throw std::runtime_error("Create device failed");
		}
		adapter->Release();
		
		std::vector<rhi::Queue> queues(info.queueCreateInfoCount);
		for (uint32_t i = 0; i < info.queueCreateInfoCount; i++) {
			D3D12_COMMAND_QUEUE_DESC queueDesc = {};
			queueDesc.Type = D3D12EnumCommandListType(info.pQueueCreateInfos[i].queueType).Get();

			decltype(D3D12WrapperQueue::queue) queue;
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
		device->CheckFeatureSupport(D3D12_FEATURE_MULTISAMPLE_QUALITY_LEVELS, &deviceWrapper->feature.sampleQuality, sizeof(UINT));
		return deviceWrapper;
	}

	ShaderCompiler D3D12WrapperInstance::CreateShaderCompiler() const {
		ShaderCompiler shaderCompiler;
		shaderCompiler.AddArgument(ShaderCompileArgument::matrix_column_major);
		shaderCompiler.AddArgument(ShaderCompileArgument::enable_debug_info);
		shaderCompiler.AddArgument(ShaderCompileArgument::treat_warnings_as_errors);
		shaderCompiler.AddIncludePath(StringToWString(GlobalConfig::configPath).c_str());
		return shaderCompiler;
	}


	/* -------------------- D3D12WrapperDevice -------------------- */


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
			decltype(D3D12WrapperImage::resource) resource;
			swapchain->GetBuffer(static_cast<UINT>(i), IID_PPV_ARGS(&resource));
			
			auto imageWrapper = new D3D12WrapperImage();
			imageWrapper->resource = resource;
			swapchainImages[i] = imageWrapper;
		}

		auto swapchainWrapper = new D3D12WrapperSwapchain();
		swapchainWrapper->queue = static_cast<D3D12WrapperQueue*>(info.queue)->queue;
		swapchainWrapper->swapchain = swapchain;
		swapchainWrapper->images = swapchainImages;
		swapchainWrapper->imageExtent = rhi::Extent2D().SetWidth(swapchainDesc.Width).SetHeight(swapchainDesc.Height);
		return swapchainWrapper;
	}

	rhi::CommandPool D3D12WrapperDevice::CreateCommandPool(const rhi::CommandPoolCreateInfo& info) const {
		auto cmdPoolWrapper = new D3D12WrapperCommandPool();
		cmdPoolWrapper->device = device;
		cmdPoolWrapper->commandListType = static_cast<D3D12WrapperQueue*>(info.queue)->commandListType;
		return cmdPoolWrapper;
	}

	rhi::Buffer D3D12WrapperDevice::CreateBuffer(const rhi::BufferCreateInfo& info) const {
		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_UPLOAD);
		auto resourceDesc = CD3DX12_RESOURCE_DESC::Buffer(info.size);

		decltype(D3D12WrapperBuffer::resource) resource;
		if (FAILED(device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12EnumResourceStates(info.usage).Get(),
			nullptr,
			IID_PPV_ARGS(&resource)))) {
			throw std::runtime_error("Create buffer failed");
		}
		
		auto bufferWrapper = new D3D12WrapperBuffer();
		bufferWrapper->resource = resource;
		bufferWrapper->bufferSize = static_cast<UINT>(info.size);
		return bufferWrapper;
	}

	rhi::Image D3D12WrapperDevice::CreateImage(const rhi::ImageCreateInfo& info) const {
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
		resourceDesc.SampleDesc.Quality = info.sampleCount == rhi::SampleCount::Count1 ? 0 : feature.sampleQuality;

		auto heapProperties = CD3DX12_HEAP_PROPERTIES(D3D12_HEAP_TYPE_DEFAULT);

		decltype(D3D12WrapperImage::resource) resource;
		if (FAILED(device->CreateCommittedResource(
			&heapProperties,
			D3D12_HEAP_FLAG_NONE,
			&resourceDesc,
			D3D12EnumResourceStates(info.initialLayout).Get(),
			nullptr,
			IID_PPV_ARGS(&resource)))) {
			throw std::runtime_error("Create image failed");
		}

		auto imageWrapper = new D3D12WrapperImage();
		imageWrapper->resource = resource;
		return imageWrapper;
	}

	rhi::Sampler D3D12WrapperDevice::CreateSampler(const rhi::SamplerCreateInfo& info) const {
		auto samplerWrapper = new D3D12WrapperSampler();
		samplerWrapper->samplerInfo = info;
		return samplerWrapper;
	}

	rhi::DescriptorPool D3D12WrapperDevice::CreateDescriptorPool(const rhi::DescriptorPoolCreateInfo& info) const {
		uint32_t cbvSrvUavDescriptorCount = 0;
		uint32_t samplerDescriptorCount = 0;

		for (uint32_t i = 0; i < info.poolSizeCount; i++) {
			if (info.pPoolSizes[i].type == rhi::DescriptorType::Sampler) {
				samplerDescriptorCount += info.pPoolSizes[i].size;
			}
			else {
				cbvSrvUavDescriptorCount += info.pPoolSizes[i].size;
			}
		}

		auto descriptorPool = new D3D12WrapperDescriptorPool();

		if (cbvSrvUavDescriptorCount > 0) {
			D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
			descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV;
			descriptorHeapDesc.NumDescriptors = cbvSrvUavDescriptorCount;

			if (FAILED(device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorPool->cbvSrvUavHeap)))) {
				throw std::runtime_error("Create descriptor heap failed");
			}
		}
		
		if (samplerDescriptorCount > 0) {
			D3D12_DESCRIPTOR_HEAP_DESC descriptorHeapDesc = {};
			descriptorHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER;
			descriptorHeapDesc.NumDescriptors = samplerDescriptorCount;

			if (FAILED(device->CreateDescriptorHeap(&descriptorHeapDesc, IID_PPV_ARGS(&descriptorPool->samplerHeap)))) {
				throw std::runtime_error("Create descriptor heap failed");
			}
		}

		return descriptorPool;
	}

	rhi::DescriptorSetLayout D3D12WrapperDevice::CreateDescriptorSetLayout(const rhi::DescriptorSetLayoutCreateInfo& info) const {
		std::vector<rhi::DescriptorSetLayoutBinding> bindings(info.bindingCount);
		std::copy(info.pBindings, info.pBindings + info.bindingCount, bindings.data());

		auto descriptorSetLayoutWrapper = new D3D12WrapperDescriptorSetLayout();
		descriptorSetLayoutWrapper->bindings = bindings;
		descriptorSetLayoutWrapper->shaderVisibility = D3D12EnumShaderVisibility(info.shaderStage).Get();
		return descriptorSetLayoutWrapper;
	}

	rhi::PipelineLayout D3D12WrapperDevice::CreatePipelineLayout(const rhi::PipelineLayoutCreateInfo& info) const {
		std::vector<D3D12_DESCRIPTOR_RANGE1> ranges;
		D3D12_DESCRIPTOR_RANGE1* pRanges = ranges.data();
		std::vector<D3D12_STATIC_SAMPLER_DESC> staticSamplerDescs;
		std::vector<D3D12_ROOT_PARAMETER1> parameters(info.descriptorSetLayoutCount);
		for (uint32_t i = 0; i < info.descriptorSetLayoutCount; i++) {
			auto descriptorSetLayout = static_cast<D3D12WrapperDescriptorSetLayout*>(info.pDescriptorSetLayouts[i]);
			UINT rangeCount = 0;
			for (auto& binding : descriptorSetLayout->bindings) {
				if (binding.pStaticSamplers) {
					for (uint32_t j = 0; j < binding.descriptorCount; j++) {
						auto& samplerInfo = static_cast<D3D12WrapperSampler*>(binding.pStaticSamplers[j])->samplerInfo;
						D3D12_STATIC_SAMPLER_DESC samplerDesc = {};
						samplerDesc.Filter = D3D12EnumFilter(
							samplerInfo.minFilter,
							samplerInfo.magFilter,
							samplerInfo.mipFilter,
							samplerInfo.compareEnable,
							samplerInfo.anisotropyEnable).Get();
						samplerDesc.AddressU = D3D12EnumTextureAddressMode(samplerInfo.addressModeU).Get();
						samplerDesc.AddressV = D3D12EnumTextureAddressMode(samplerInfo.addressModeV).Get();
						samplerDesc.AddressW = D3D12EnumTextureAddressMode(samplerInfo.addressModeW).Get();
						samplerDesc.MipLODBias = samplerInfo.mipLodBias;
						samplerDesc.MinLOD = samplerInfo.minLod;
						samplerDesc.MaxLOD = samplerInfo.maxLod;
						samplerDesc.MaxAnisotropy = static_cast<UINT>(samplerInfo.maxAnisotropy);
						samplerDesc.ComparisonFunc = D3D12EnumComparisonFunc(samplerInfo.compareOp).Get();
						samplerDesc.BorderColor = D3D12EnumStaticBorderColor(samplerInfo.borderColor).Get();
						samplerDesc.ShaderRegister = binding.binding + j;
						samplerDesc.RegisterSpace = i;
						samplerDesc.ShaderVisibility = descriptorSetLayout->shaderVisibility;
						staticSamplerDescs.emplace_back(samplerDesc);
						/*switch (samplerInfo.borderColor) {
						case rhi::BorderColor::TransparentBlack:
							samplerDesc.BorderColor[0] = 0.0f;
							samplerDesc.BorderColor[1] = 0.0f;
							samplerDesc.BorderColor[2] = 0.0f;
							samplerDesc.BorderColor[3] = 0.0f;
							break;
						case rhi::BorderColor::OpaqueBlack:
							samplerDesc.BorderColor[0] = 0.0f;
							samplerDesc.BorderColor[1] = 0.0f;
							samplerDesc.BorderColor[2] = 0.0f;
							samplerDesc.BorderColor[3] = 1.0f;
							break;
						case rhi::BorderColor::OpaqueWhite:
							samplerDesc.BorderColor[0] = 1.0f;
							samplerDesc.BorderColor[1] = 1.0f;
							samplerDesc.BorderColor[2] = 1.0f;
							samplerDesc.BorderColor[3] = 1.0f;
							break;
						}*/

					}
				}
				else {
					ranges.emplace_back(CD3DX12_DESCRIPTOR_RANGE1(
						D3D12EnumDescriptorRangeType(binding.descriptorType).Get(),
						static_cast<UINT>(binding.descriptorCount),
						static_cast<UINT>(binding.binding),
						static_cast<UINT>(i)));
					rangeCount++;
				}
			}
			CD3DX12_ROOT_PARAMETER1 parameter;
			parameter.InitAsDescriptorTable(rangeCount, pRanges, descriptorSetLayout->shaderVisibility);
			parameters[i] = parameter;
		}

		CD3DX12_VERSIONED_ROOT_SIGNATURE_DESC rootSignatureDesc;
		rootSignatureDesc.Init_1_1(info.descriptorSetLayoutCount, parameters.data(), static_cast<UINT>(staticSamplerDescs.size()), staticSamplerDescs.data());

		ID3DBlob* rootSignatureBlob, * errorBlob;
		if (FAILED(D3DX12SerializeVersionedRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1_1, &rootSignatureBlob, &errorBlob))) {
			throw std::runtime_error("Serialize root signature failed");
		}
		if (errorBlob) {
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
			errorBlob->Release();
		}

		ID3D12RootSignature* rootSignature;
		if (FAILED(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)))) {
			throw std::runtime_error("Create root signature failed");
		}

		rootSignatureBlob->Release();

		auto pipelineLayoutInfo = new D3D12WrapperPipelineLayout();
		pipelineLayoutInfo->rootSignature = rootSignature;
		return pipelineLayoutInfo;
	}

	rhi::RenderPass D3D12WrapperDevice::CreateRenderPass(const rhi::RenderPassCreateInfo& info) const {
		auto renderPass = new D3D12WrapperRenderPass();
		renderPass->colorAttachmentCount = info.colorAttachmentCount;

		if (info.colorAttachmentCount != 0) {
			renderPass->pColorAttachments = new rhi::AttachmentDescription[info.colorAttachmentCount];
			std::copy(info.pColorAttachments, info.pColorAttachments + info.colorAttachmentCount, renderPass->pColorAttachments);
		}
		if (info.pDepthStencilAttachment) {
			renderPass->pDepthStencilAttachment = new rhi::AttachmentDescription();
			*renderPass->pDepthStencilAttachment = *info.pDepthStencilAttachment;
		}
		
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
		std::vector<D3D12_INPUT_ELEMENT_DESC> inputElementDescs(info.vertexInputInfo.attributeCount);
		for (uint32_t i = 0; i < info.vertexInputInfo.attributeCount; i++) {
			auto bindingSlot = info.vertexInputInfo.pAttributes[i].binding;
			D3D12_INPUT_ELEMENT_DESC inputElementDesc = {};
			inputElementDesc.SemanticName = info.vertexInputInfo.pAttributes[i].semantic;
			inputElementDesc.InputSlot = bindingSlot;
			inputElementDesc.Format = D3D12EnumFormat(info.vertexInputInfo.pAttributes[i].format).Get();
			inputElementDesc.AlignedByteOffset = info.vertexInputInfo.pAttributes[i].offset;
			inputElementDesc.InputSlotClass = D3D12EnumInputClassification(info.vertexInputInfo.pBindings[bindingSlot].inputRate).Get();
			inputElementDescs[i] = inputElementDesc;
		}

		D3D12_INPUT_LAYOUT_DESC inputLayoutDesc = {};
		inputLayoutDesc.NumElements = static_cast<UINT>(info.vertexInputInfo.attributeCount);
		inputLayoutDesc.pInputElementDescs = inputElementDescs.data();
		
		D3D12_RASTERIZER_DESC rasterizationDesc = {};
		rasterizationDesc.AntialiasedLineEnable = info.rasterizationInfo.smoothLine;
		rasterizationDesc.CullMode = D3D12EnumCullMode(info.rasterizationInfo.cullMode).Get();
		rasterizationDesc.FillMode = D3D12EnumFillMode(info.rasterizationInfo.polygonMode).Get();
		rasterizationDesc.DepthBias = static_cast<INT>(info.rasterizationInfo.depthBias);
		rasterizationDesc.DepthBiasClamp = info.rasterizationInfo.depthClamp;
		rasterizationDesc.SlopeScaledDepthBias = info.rasterizationInfo.slopeScaledDepthBias;
		rasterizationDesc.MultisampleEnable = info.multisampleInfo.rasterizationSamples == rhi::SampleCount::Count1 ? FALSE : TRUE;
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
		blendDesc.IndependentBlendEnable = TRUE;
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
		pipelineDesc.inputLayoutDesc.value = inputLayoutDesc;
		pipelineDesc.rasterizationDesc.value = rasterizationDesc;
		pipelineDesc.depthStencilDesc.value = depthStencilDesc;
		pipelineDesc.blendDesc.value = blendDesc;
		pipelineDesc.primitiveTopologyType.value = D3D12EnumPrimitiveTopologyType(info.inputAssemblyInfo.topology).Get();
		pipelineDesc.ibStripCutValue.value = info.inputAssemblyInfo.primitivieRestart ?
			D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_0xFFFFFFFF :
			D3D12_INDEX_BUFFER_STRIP_CUT_VALUE_DISABLED;
		pipelineDesc.sampleDesc.value.Count = 1;
		pipelineDesc.sampleDesc.value.Quality = 0;

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
		
		D3D12_ROOT_SIGNATURE_DESC rootSignatureDesc = {};
		rootSignatureDesc.Flags = D3D12_ROOT_SIGNATURE_FLAG_ALLOW_INPUT_ASSEMBLER_INPUT_LAYOUT;
		ID3DBlob* rootSignatureBlob, * errorBlob;
		if (FAILED(D3D12SerializeRootSignature(&rootSignatureDesc, D3D_ROOT_SIGNATURE_VERSION_1, &rootSignatureBlob, &errorBlob))) {
			throw std::runtime_error("Serialize root signature failed");
		}
		if (errorBlob) {
			OutputDebugStringA(static_cast<char*>(errorBlob->GetBufferPointer()));
		}

		ID3D12RootSignature* rootSignature;
		if (FAILED(device->CreateRootSignature(0, rootSignatureBlob->GetBufferPointer(), rootSignatureBlob->GetBufferSize(), IID_PPV_ARGS(&rootSignature)))) {
			throw std::runtime_error("Create root signature failed");
		}
		
		pipelineDesc.rootSignature.value = rootSignature;

		D3D12_PIPELINE_STATE_STREAM_DESC streamDesc = {};
		streamDesc.SizeInBytes = sizeof(pipelineDesc);
		streamDesc.pPipelineStateSubobjectStream = &pipelineDesc;

		decltype(D3D12WrapperPipeline::pipeline) pipeline;
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
		pipelineWrapper->rootSignature = rootSignature;
		pipelineWrapper->scissors = scissors;
		pipelineWrapper->viewports = viewports;
		pipelineWrapper->primitiveTopology = info.inputAssemblyInfo.topology == rhi::PrimitiveTopology::PatchList ?
			D3DEnumPatchControlPoints(info.tessellationInfo.patchControlPoints).Get() : 
			D3DEnumPrimitiveTopology(info.inputAssemblyInfo.topology).Get();

		pipelineWrapper->vertexInputInfo.bindingStride.resize(info.vertexInputInfo.bindingCount);
		for (uint32_t i = 0; i < info.vertexInputInfo.bindingCount; i++) {
			pipelineWrapper->vertexInputInfo.bindingStride[i] = info.vertexInputInfo.pBindings[i].stride;
		}

		pipelineWrapper->depthStencilInfo.minDepthBounds = info.depthStencilInfo.maxDepthBounds;
		pipelineWrapper->depthStencilInfo.maxDepthBounds = info.depthStencilInfo.maxDepthBounds;
		return pipelineWrapper;
	}

	rhi::Framebuffer D3D12WrapperDevice::CreateFramebuffer(const rhi::FramebufferCreateInfo& info) const {
		auto framebuffer = new D3D12WrapperFramebuffer();
		framebuffer->colorAttachments.resize(info.colorAttachmentCount);
		framebuffer->rtvHandleIncrementSize = device->GetDescriptorHandleIncrementSize(D3D12_DESCRIPTOR_HEAP_TYPE_RTV);

		D3D12_DESCRIPTOR_HEAP_DESC rtvHeapDesc = {};
		rtvHeapDesc.NumDescriptors = info.colorAttachmentCount;
		rtvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_RTV;

		if (FAILED(device->CreateDescriptorHeap(&rtvHeapDesc, IID_PPV_ARGS(&framebuffer->renderTargetHeap)))) {
			throw std::runtime_error("Create descriptor heap failed");
		}

		CD3DX12_CPU_DESCRIPTOR_HANDLE rtvHandle(framebuffer->renderTargetHeap->GetCPUDescriptorHandleForHeapStart());
		for (uint32_t i = 0; i < info.colorAttachmentCount; i++) {
			auto& colorAttachment = info.pColorAttachments[i];
			framebuffer->colorAttachments[i] = static_cast<D3D12WrapperImage*>(colorAttachment.image)->resource;

			D3D12_RENDER_TARGET_VIEW_DESC rtvDesc = {};
			rtvDesc.Format = D3D12EnumFormat(colorAttachment.format).Get();
			rtvDesc.ViewDimension = D3D12EnumRTVDimension(colorAttachment.viewType).Get();

			switch (colorAttachment.viewType) {
			case rhi::ImageViewType::Image1D:
				rtvDesc.Texture1D.MipSlice = colorAttachment.baseMipLevel;
				break;
			case rhi::ImageViewType::Image2D:
				rtvDesc.Texture2D.MipSlice = colorAttachment.baseMipLevel;
				break;
			case rhi::ImageViewType::Image3D:
				rtvDesc.Texture3D.MipSlice = colorAttachment.baseMipLevel;
				rtvDesc.Texture3D.FirstWSlice = colorAttachment.baseArrayLayer;
				rtvDesc.Texture3D.WSize = colorAttachment.arrayLayerCount;
				break;
			case rhi::ImageViewType::ImageArray1D:
				rtvDesc.Texture1DArray.MipSlice = colorAttachment.baseMipLevel;
				rtvDesc.Texture1DArray.FirstArraySlice = colorAttachment.baseArrayLayer;
				rtvDesc.Texture1DArray.ArraySize = colorAttachment.arrayLayerCount;
				break;
			case rhi::ImageViewType::ImageArray2D:
				rtvDesc.Texture2DArray.MipSlice = colorAttachment.baseMipLevel;
				rtvDesc.Texture2DArray.FirstArraySlice = colorAttachment.baseArrayLayer;
				rtvDesc.Texture2DArray.ArraySize = colorAttachment.arrayLayerCount;
				break;
			}

			device->CreateRenderTargetView(framebuffer->colorAttachments[i], &rtvDesc, rtvHandle);
			rtvHandle.Offset(framebuffer->rtvHandleIncrementSize);
		}

		if (info.pDepthStencilAttachment) {
			framebuffer->depthStencilAttachment = static_cast<D3D12WrapperImage*>(info.pDepthStencilAttachment->image)->resource;

			D3D12_DESCRIPTOR_HEAP_DESC dsvHeapDesc = {};
			dsvHeapDesc.NumDescriptors = 1;
			dsvHeapDesc.Type = D3D12_DESCRIPTOR_HEAP_TYPE_DSV;

			if (FAILED(device->CreateDescriptorHeap(&dsvHeapDesc, IID_PPV_ARGS(&framebuffer->depthStencilHeap)))) {
				throw std::runtime_error("Create descriptor heap failed");
			}

			CD3DX12_CPU_DESCRIPTOR_HANDLE dsvHandle(framebuffer->depthStencilHeap->GetCPUDescriptorHandleForHeapStart());

			D3D12_DEPTH_STENCIL_VIEW_DESC dsvDesc = {};
			dsvDesc.Format = D3D12EnumFormat(info.pDepthStencilAttachment->format).Get();
			dsvDesc.ViewDimension = D3D12EnumDSVDimension(info.pDepthStencilAttachment->viewType).Get();
			
			switch (info.pDepthStencilAttachment->viewType) {
			case rhi::ImageViewType::Image1D:
				dsvDesc.Texture1D.MipSlice = info.pDepthStencilAttachment->baseMipLevel;
				break;
			case rhi::ImageViewType::Image2D:
				dsvDesc.Texture2D.MipSlice = info.pDepthStencilAttachment->baseMipLevel;
				break;
			case rhi::ImageViewType::ImageArray1D:
				dsvDesc.Texture1DArray.MipSlice = info.pDepthStencilAttachment->baseMipLevel;
				dsvDesc.Texture1DArray.FirstArraySlice = info.pDepthStencilAttachment->baseArrayLayer;
				dsvDesc.Texture1DArray.ArraySize = info.pDepthStencilAttachment->arrayLayerCount;
				break;
			case rhi::ImageViewType::ImageArray2D:
				dsvDesc.Texture2DArray.MipSlice = info.pDepthStencilAttachment->baseMipLevel;
				dsvDesc.Texture2DArray.FirstArraySlice = info.pDepthStencilAttachment->baseArrayLayer;
				dsvDesc.Texture2DArray.ArraySize = info.pDepthStencilAttachment->arrayLayerCount;
				break;
			}

			device->CreateDepthStencilView(framebuffer->depthStencilAttachment, &dsvDesc, dsvHandle);
		}

		return framebuffer;
	}

	rhi::Fence D3D12WrapperDevice::CreateFence(const rhi::FenceCreateInfo& info) const {
		decltype(D3D12WrapperFence::fence) fence;
		if (FAILED(device->CreateFence(0, D3D12_FENCE_FLAG_NONE, IID_PPV_ARGS(&fence)))) {
			throw std::runtime_error("Create fence failed");
		}

		auto fenceWrapper = new D3D12WrapperFence();
		fenceWrapper->fence = fence;
		return fenceWrapper;
	}


	/* -------------------- D3D12WrapperQueue -------------------- */


	void D3D12WrapperQueue::SubmitCommandBuffers(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) {
		std::vector<ID3D12CommandList*> submitCommandBuffers(commandBufferCount);
		for (uint32_t i = 0; i < commandBufferCount; i++) {
			submitCommandBuffers[i] = static_cast<D3D12WrapperCommandBuffer*>(commandBuffers[i])->commandList;
		}
		queue->ExecuteCommandLists(commandBufferCount, submitCommandBuffers.data());

		queue->Signal(static_cast<D3D12WrapperFence*>(fence)->fence, TRUE);
	}

	void D3D12WrapperQueue::PresentSwapchain(rhi::Swapchain swapchain, uint32_t imageIndex) {
		static_cast<D3D12WrapperSwapchain*>(swapchain)->swapchain->Present(0, 0);
	}


	/* -------------------- D3D12WrapperSwapchain -------------------- */


	rhi::Extent2D D3D12WrapperSwapchain::GetImageExtent() const {
		return imageExtent;
	}

	std::vector<rhi::Image> D3D12WrapperSwapchain::GetImages() const {
		return images;
	}

	uint32_t D3D12WrapperSwapchain::AcquireNextImage(rhi::Fence fence) {
		currentImageIndex = swapchain->GetCurrentBackBufferIndex();
		queue->Signal(static_cast<D3D12WrapperFence*>(fence)->fence, TRUE);
		return currentImageIndex;
	}


	/* -------------------- D3D12WrapperCommandPool -------------------- */


	void D3D12WrapperCommandPool::Reset() {
		for (auto& commandBuffer : commandBuffers) {
			static_cast<D3D12WrapperCommandBuffer*>(commandBuffer)->commandAllocator->Reset();
		}
	}

	std::vector<rhi::CommandBuffer> D3D12WrapperCommandPool::AllocateCommandBuffers(uint32_t bufferCount) {
		std::vector<rhi::CommandBuffer> cmdWrappers(bufferCount);
		for (uint32_t i = 0; i < bufferCount; i++) {
			decltype(D3D12WrapperCommandBuffer::commandAllocator) cmdAllocator;
			if (FAILED(device->CreateCommandAllocator(commandListType, IID_PPV_ARGS(&cmdAllocator)))) {
				throw std::runtime_error("Create command allocator failed");
			}

			decltype(D3D12WrapperCommandBuffer::commandList) commandList;
			if (FAILED(device->CreateCommandList(0, commandListType, cmdAllocator, nullptr, IID_PPV_ARGS(&commandList)))) {
				throw std::runtime_error("Create graphics command list failed");
			}

			commandList->Close();

			auto cmdWrapper = new D3D12WrapperCommandBuffer();
			cmdWrapper->commandAllocator = cmdAllocator;
			cmdWrapper->commandList = commandList;
			cmdWrappers[i] = cmdWrapper;
		}
		commandBuffers.insert(commandBuffers.end(), cmdWrappers.begin(), cmdWrappers.end());

		return cmdWrappers;
	}


	/* -------------------- D3D12WrapperCommandBuffer -------------------- */


	void D3D12WrapperCommandBuffer::Reset() {
		commandAllocator->Reset();
	}

	void D3D12WrapperCommandBuffer::Begin(const rhi::CommandBufferBeginInfo& info) {
		commandList->Reset(commandAllocator, nullptr);
	}

	void D3D12WrapperCommandBuffer::End() {
		commandList->Close();
	}

	void D3D12WrapperCommandBuffer::BeginRenderPass(const rhi::RenderPassBeginInfo& info) {
		auto renderPass = static_cast<D3D12WrapperRenderPass*>(info.renderPass);
		auto framebuffer = static_cast<D3D12WrapperFramebuffer*>(info.framebuffer);

		std::vector<D3D12_RESOURCE_BARRIER> resourceBarriers(renderPass->colorAttachmentCount);
		for (uint32_t i = 0; i < renderPass->colorAttachmentCount; i++) {
			resourceBarriers[i] = CD3DX12_RESOURCE_BARRIER::Transition(
				framebuffer->colorAttachments[i],
				D3D12EnumResourceStates(renderPass->pColorAttachments[i].initialLayout).Get(),
				D3D12EnumResourceStates(renderPass->pColorAttachments[i].passLayout).Get());
		}
		if (renderPass->pDepthStencilAttachment) {
			resourceBarriers.emplace_back(CD3DX12_RESOURCE_BARRIER::Transition(
				framebuffer->depthStencilAttachment,
				D3D12EnumResourceStates(renderPass->pDepthStencilAttachment->initialLayout).Get(),
				D3D12EnumResourceStates(renderPass->pDepthStencilAttachment->passLayout).Get()));
		}

		commandList->ResourceBarrier(static_cast<UINT>(resourceBarriers.size()), resourceBarriers.data());

		renderPassResourceBarriers.resize(renderPass->colorAttachmentCount);
		for (uint32_t i = 0; i < renderPass->colorAttachmentCount; i++) {
			renderPassResourceBarriers[i] = CD3DX12_RESOURCE_BARRIER::Transition(
				framebuffer->colorAttachments[i],
				D3D12EnumResourceStates(renderPass->pColorAttachments[i].passLayout).Get(),
				D3D12EnumResourceStates(renderPass->pColorAttachments[i].finalLayout).Get());
		}
		if (renderPass->pDepthStencilAttachment) {
			renderPassResourceBarriers.emplace_back(CD3DX12_RESOURCE_BARRIER::Transition(
				framebuffer->depthStencilAttachment,
				D3D12EnumResourceStates(renderPass->pDepthStencilAttachment->passLayout).Get(),
				D3D12EnumResourceStates(renderPass->pDepthStencilAttachment->finalLayout).Get()));
		}

		std::vector<D3D12_RENDER_PASS_RENDER_TARGET_DESC> renderTargetDescs(renderPass->colorAttachmentCount);
		for (uint32_t i = 0; i < renderPass->colorAttachmentCount; i++) {
			FLOAT colorRGBA[4] = {
				info.pClearColorValues[i].colorValue[0],
				info.pClearColorValues[i].colorValue[1],
				info.pClearColorValues[i].colorValue[2],
				info.pClearColorValues[i].colorValue[3]
			};

			D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS clearParameters = {};
			clearParameters.ClearValue = CD3DX12_CLEAR_VALUE(
				D3D12EnumFormat(renderPass->pColorAttachments[i].format).Get(),
				colorRGBA
			);

			D3D12_RENDER_PASS_RENDER_TARGET_DESC renderTargetDesc = {};
			renderTargetDesc.cpuDescriptor = CD3DX12_CPU_DESCRIPTOR_HANDLE(
				framebuffer->renderTargetHeap->GetCPUDescriptorHandleForHeapStart(), i, framebuffer->rtvHandleIncrementSize);
			renderTargetDesc.BeginningAccess.Type = D3D12EnumRenderPassBeginningAccessType(renderPass->pColorAttachments[i].loadOp).Get();
			renderTargetDesc.BeginningAccess.Clear = clearParameters;
			renderTargetDesc.EndingAccess.Type = D3D12EnumRenderPassEndingAccessType(renderPass->pColorAttachments[i].storeOp).Get();
			renderTargetDescs[i] = renderTargetDesc;
		}

		D3D12_RENDER_PASS_DEPTH_STENCIL_DESC depthStencilDesc = {};
		if (renderPass->pDepthStencilAttachment) {
			D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS depthClearParameters = {};
			depthClearParameters.ClearValue = CD3DX12_CLEAR_VALUE(
				D3D12EnumFormat(renderPass->pDepthStencilAttachment->format).Get(),
				info.clearDepthStencilValue.depthValue, 0
			);

			D3D12_RENDER_PASS_BEGINNING_ACCESS_CLEAR_PARAMETERS stencilClearParameters = {};
			stencilClearParameters.ClearValue = CD3DX12_CLEAR_VALUE(
				D3D12EnumFormat(renderPass->pDepthStencilAttachment->format).Get(),
				0.0f, info.clearDepthStencilValue.stencilValue
			);

			depthStencilDesc.cpuDescriptor = framebuffer->depthStencilHeap->GetCPUDescriptorHandleForHeapStart();
			depthStencilDesc.DepthBeginningAccess.Type = D3D12EnumRenderPassBeginningAccessType(renderPass->pDepthStencilAttachment->loadOp).Get();
			depthStencilDesc.DepthBeginningAccess.Clear = depthClearParameters;
			depthStencilDesc.DepthEndingAccess.Type = D3D12EnumRenderPassEndingAccessType(renderPass->pDepthStencilAttachment->storeOp).Get();
			depthStencilDesc.StencilBeginningAccess.Type = D3D12EnumRenderPassBeginningAccessType(renderPass->pDepthStencilAttachment->stencilLoadOp).Get();
			depthStencilDesc.StencilBeginningAccess.Clear = stencilClearParameters;
			depthStencilDesc.StencilEndingAccess.Type = D3D12EnumRenderPassEndingAccessType(renderPass->pDepthStencilAttachment->stencilStoreOp).Get();
		}

		commandList->BeginRenderPass(
			renderPass->colorAttachmentCount, 
			renderTargetDescs.data(),
			renderPass->pDepthStencilAttachment ? &depthStencilDesc : nullptr,
			D3D12_RENDER_PASS_FLAG_NONE);
	}

	void D3D12WrapperCommandBuffer::EndRenderPass() {
		commandList->EndRenderPass();
		commandList->ResourceBarrier(static_cast<UINT>(renderPassResourceBarriers.size()), renderPassResourceBarriers.data());
		if (pipeline) pipeline = nullptr;
		decltype(renderPassResourceBarriers) temp;
		temp.swap(renderPassResourceBarriers);
	}

	void D3D12WrapperCommandBuffer::BindPipeline(const rhi::Pipeline& pipeline) {
		this->pipeline = static_cast<D3D12WrapperPipeline*>(pipeline);
		auto pipelineState = static_cast<D3D12WrapperPipeline*>(pipeline);
		commandList->SetPipelineState(pipelineState->pipeline);
		commandList->RSSetScissorRects(static_cast<UINT>(pipelineState->scissors.size()), pipelineState->scissors.data());
		commandList->RSSetViewports(static_cast<UINT>(pipelineState->viewports.size()), pipelineState->viewports.data());
		commandList->OMSetDepthBounds(pipelineState->depthStencilInfo.minDepthBounds, pipelineState->depthStencilInfo.maxDepthBounds);
		commandList->IASetPrimitiveTopology(pipelineState->primitiveTopology);
		commandList->SetGraphicsRootSignature(pipelineState->rootSignature);
	}

	void D3D12WrapperCommandBuffer::BindVertexBuffer(uint32_t firstBinding, uint32_t bindingCount, const rhi::Buffer* pBuffers) {
		vertexBindingInfo.vertexBufferViews.resize(bindingCount);
		for (uint32_t i = 0; i < bindingCount; i++) {
			auto vertexBuffer = static_cast<D3D12WrapperBuffer*>(pBuffers[i]);
			vertexBindingInfo.vertexBufferViews[i].BufferLocation = vertexBuffer->resource->GetGPUVirtualAddress();
			vertexBindingInfo.vertexBufferViews[i].SizeInBytes = vertexBuffer->bufferSize;
		}
		vertexBindingInfo.firstBinding = firstBinding;
		vertexBindingInfo.bindingCount = bindingCount;
		vertexBindingInfo.dirty = true;
	}

	void D3D12WrapperCommandBuffer::BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) {
		auto indexBuffer = static_cast<D3D12WrapperBuffer*>(buffer);
		D3D12_INDEX_BUFFER_VIEW indexBufferView = {};
		indexBufferView.BufferLocation = indexBuffer->resource->GetGPUVirtualAddress();
		indexBufferView.SizeInBytes = indexBuffer->bufferSize;
		indexBufferView.Format = D3D12EnumIndexType(indexType).Get();
		commandList->IASetIndexBuffer(&indexBufferView);
	}

	void D3D12WrapperCommandBuffer::PrepareRenderingInfo() {
		if (vertexBindingInfo.dirty) {
			for (uint32_t i = 0; i < vertexBindingInfo.bindingCount; i++) {
				vertexBindingInfo.vertexBufferViews[i].StrideInBytes = pipeline->vertexInputInfo.bindingStride[i];
			}
			commandList->IASetVertexBuffers(vertexBindingInfo.firstBinding, vertexBindingInfo.bindingCount, vertexBindingInfo.vertexBufferViews.data());

			vertexBindingInfo.dirty = false;

			decltype(vertexBindingInfo.vertexBufferViews) temp;
			temp.swap(vertexBindingInfo.vertexBufferViews);
		}
	}

	void D3D12WrapperCommandBuffer::Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) {
		PrepareRenderingInfo();
		commandList->DrawInstanced(vertexCount, instanceCount, firstVertex, firstInstance);
	}

	void D3D12WrapperCommandBuffer::DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) {
		PrepareRenderingInfo();
		commandList->DrawIndexedInstanced(indexCount, instanceCount, firstIndex, vertexOffset, firstInstance);
	}


	/* -------------------- D3D12WrapperBuffer -------------------- */


	void* D3D12WrapperBuffer::Map() {
		range = {};

		void* pData;
		if (FAILED(resource->Map(0, nullptr, &pData))) {
			throw std::runtime_error("Map the resource failed");
		}

		return pData;
	}

	void* D3D12WrapperBuffer::Map(uint64_t offset, uint64_t size) {
		range.Begin = static_cast<SIZE_T>(offset);
		range.End = static_cast<SIZE_T>(size);

		void* pData;
		if (FAILED(resource->Map(0, &range, &pData))) {
			throw std::runtime_error("Map the resource failed");
		}

		return pData;
	}

	void D3D12WrapperBuffer::Unmap() {
		if (range.End == 0) {
			resource->Unmap(0, nullptr);
		}
		else {
			resource->Unmap(0, &range);
		}
	}


	/* -------------------- D3D12WrapperFence -------------------- */


	void D3D12WrapperFence::Reset() {
		fence->Signal(FALSE);
	}

	void D3D12WrapperFence::Wait() {
		if (!fence->GetCompletedValue()) {
			HANDLE eventHandle = CreateEventEx(nullptr, nullptr, FALSE, EVENT_ALL_ACCESS);
			fence->SetEventOnCompletion(TRUE, eventHandle);

			if (!eventHandle) {
				throw std::runtime_error("Create event failed");
			}

			WaitForSingleObject(eventHandle, INFINITE);
			CloseHandle(eventHandle);
		}
	}

	bool D3D12WrapperFence::GetCurrentStatus() const {
		return fence->GetCompletedValue();
	}


	/* -------------------- destroy handle -------------------- */


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
		for (auto& commandBuffer : commandBuffers) {
			auto cmdBuffer = static_cast<D3D12WrapperCommandBuffer*>(commandBuffer);
			cmdBuffer->commandAllocator->Release();
			cmdBuffer->commandList->Release();
			delete commandBuffer;
		}
		delete this;
	}

	void D3D12WrapperBuffer::Destroy() {
		resource->Release();
		delete this;
	}

	void D3D12WrapperImage::Destroy() {
		resource->Release();
		delete this;
	}

	void D3D12WrapperSampler::Destroy() {
		delete this;
	}

	void D3D12WrapperDescriptorPool::Destroy() {
		if (cbvSrvUavHeap) cbvSrvUavHeap->Release();
		if (samplerHeap) samplerHeap->Release();
		delete this;
	}

	void D3D12WrapperDescriptorSetLayout::Destroy() {
		delete this;
	}

	void D3D12WrapperPipelineLayout::Destroy() {
		rootSignature->Release();
		delete this;
	}

	void D3D12WrapperRenderPass::Destroy() {
		if (pColorAttachments) delete[] pColorAttachments;
		if (pDepthStencilAttachment) delete pColorAttachments;
		delete this;
	}

	void D3D12WrapperShaderModule::Destroy() {
		delete this;
	}

	void D3D12WrapperPipeline::Destroy() {
		pipeline->Release();
		rootSignature->Release();
		delete this;
	}

	void D3D12WrapperFramebuffer::Destroy() {
		if (renderTargetHeap) renderTargetHeap->Release();
		if (depthStencilHeap) depthStencilHeap->Release();
		delete this;
	}

	void D3D12WrapperFence::Destroy() {
		fence->Release();
		delete this;
	}

}

#endif // _DIRECT3D12