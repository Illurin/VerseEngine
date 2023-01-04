#pragma once

#include "d3d12_util.hpp"

#ifdef _DIRECT3D12

namespace engine {

	class D3D12EnumFormat final {
	public:
		D3D12EnumFormat(RHIFormat format) {
			switch (format) {
			case RHIFormat::Undefined: format_ = DXGI_FORMAT_UNKNOWN; break;
			case RHIFormat::R8Unorm: format_ = DXGI_FORMAT_R8_UNORM; break;
			case RHIFormat::R8G8Unorm: format_ = DXGI_FORMAT_R8G8_UNORM; break;
			//case RHIFormat::R8G8B8Unorm: format_ = DXGI_FORMAT_R8G8B8_UNORM; break;
			case RHIFormat::R8G8B8A8Unorm: format_ = DXGI_FORMAT_R8G8B8A8_UNORM; break;
			case RHIFormat::R8Snorm: format_ = DXGI_FORMAT_R8_SNORM; break;
			case RHIFormat::R8G8Snorm: format_ = DXGI_FORMAT_R8G8_SNORM; break;
			//case RHIFormat::R8G8B8Snorm: format_ = DXGI_FORMAT_R8G8B8_SNORM; break;
			case RHIFormat::R8G8B8A8Snorm: format_ = DXGI_FORMAT_R8G8B8A8_SNORM; break;
			case RHIFormat::R8Uint: format_ = DXGI_FORMAT_R8_UINT; break;
			case RHIFormat::R8G8Uint: format_ = DXGI_FORMAT_R8G8_UINT; break;
			//case RHIFormat::R8G8B8Uint: format_ = DXGI_FORMAT_R8G8B8_UINT; break;
			case RHIFormat::R8G8B8A8Uint: format_ = DXGI_FORMAT_R8G8B8A8_UINT; break;
			case RHIFormat::R8Sint: format_ = DXGI_FORMAT_R8_SINT; break;
			case RHIFormat::R8G8Sint: format_ = DXGI_FORMAT_R8G8_SINT; break;
			//case RHIFormat::R8G8B8Sint: format_ = DXGI_FORMAT_R8G8B8_SINT; break;
			case RHIFormat::R8G8B8A8Sint: format_ = DXGI_FORMAT_R8G8B8A8_SINT; break;
			case RHIFormat::R16Unorm: format_ = DXGI_FORMAT_R16_UNORM; break;
			case RHIFormat::R16G16Unorm: format_ = DXGI_FORMAT_R16G16_UNORM; break;
			//case RHIFormat::R16G16B16Unorm: format_ = DXGI_FORMAT_R16G16B16_UNORM; break;
			case RHIFormat::R16G16B16A16Unorm: format_ = DXGI_FORMAT_R16G16B16A16_UNORM; break;
			case RHIFormat::R16Snorm: format_ = DXGI_FORMAT_R16_SNORM; break;
			case RHIFormat::R16G16Snorm: format_ = DXGI_FORMAT_R16G16_SNORM; break;
			//case RHIFormat::R16G16B16Snorm: format_ = DXGI_FORMAT_R16G16B16_SNORM; break;
			case RHIFormat::R16G16B16A16Snorm: format_ = DXGI_FORMAT_R16G16B16A16_SNORM; break;
			case RHIFormat::R16Uint: format_ = DXGI_FORMAT_R16_UINT; break;
			case RHIFormat::R16G16Uint: format_ = DXGI_FORMAT_R16G16_UINT; break;
			//case RHIFormat::R16G16B16Uint: format_ = DXGI_FORMAT_R16G16B16_UINT; break;
			case RHIFormat::R16G16B16A16Uint: format_ = DXGI_FORMAT_R16G16B16A16_UINT; break;
			case RHIFormat::R16Sint: format_ = DXGI_FORMAT_R16_SINT; break;
			case RHIFormat::R16G16Sint: format_ = DXGI_FORMAT_R16G16_SINT; break;
			//case RHIFormat::R16G16B16Sint: format_ = DXGI_FORMAT_R16G16B16_SINT; break;
			case RHIFormat::R16G16B16A16Sint: format_ = DXGI_FORMAT_R16G16B16A16_SINT; break;
			case RHIFormat::R16Sfloat: format_ = DXGI_FORMAT_R16_FLOAT; break;
			case RHIFormat::R16G16Sfloat: format_ = DXGI_FORMAT_R16G16_FLOAT; break;
			//case RHIFormat::R16G16B16Sfloat: format_ = DXGI_FORMAT_R16G16B16_FLOAT; break;
			case RHIFormat::R16G16B16A16Sfloat: format_ = DXGI_FORMAT_R16G16B16A16_FLOAT; break;
			case RHIFormat::R32Uint: format_ = DXGI_FORMAT_R32_UINT; break;
			case RHIFormat::R32G32Uint: format_ = DXGI_FORMAT_R32G32_UINT; break;
			case RHIFormat::R32G32B32Uint: format_ = DXGI_FORMAT_R32G32B32_UINT; break;
			case RHIFormat::R32G32B32A32Uint: format_ = DXGI_FORMAT_R32G32B32A32_UINT; break;
			case RHIFormat::R32Sint: format_ = DXGI_FORMAT_R32_SINT; break;
			case RHIFormat::R32G32Sint: format_ = DXGI_FORMAT_R32G32_SINT; break;
			case RHIFormat::R32G32B32Sint: format_ = DXGI_FORMAT_R32G32B32_SINT; break;
			case RHIFormat::R32G32B32A32Sint: format_ = DXGI_FORMAT_R32G32B32A32_SINT; break;
			case RHIFormat::R32Sfloat: format_ = DXGI_FORMAT_R32_FLOAT; break;
			case RHIFormat::R32G32Sfloat: format_ = DXGI_FORMAT_R32G32_FLOAT; break;
			case RHIFormat::R32G32B32Sfloat: format_ = DXGI_FORMAT_R32G32B32_FLOAT; break;
			case RHIFormat::R32G32B32A32Sfloat: format_ = DXGI_FORMAT_R32G32B32A32_FLOAT; break;
			//case RHIFormat::R64Uint: format_ = DXGI_FORMAT_R64_UINT; break;
			//case RHIFormat::R64G64Uint: format_ = DXGI_FORMAT_R64G64_UINT; break;
			//case RHIFormat::R64G64B64Uint: format_ = DXGI_FORMAT_R64G64B64_UINT; break;
			//case RHIFormat::R64G64B64A64Uint: format_ = DXGI_FORMAT_R64G64B64A64_UINT; break;
			//case RHIFormat::R64Sint: format_ = DXGI_FORMAT_R64_SINT; break;
			//case RHIFormat::R64G64Sint: format_ = DXGI_FORMAT_R64G64_SINT; break;
			//case RHIFormat::R64G64B64Sint: format_ = DXGI_FORMAT_R64G64B64_SINT; break;
			//case RHIFormat::R64G64B64A64Sint: format_ = DXGI_FORMAT_R64G64B64A64_SINT; break;
			//case RHIFormat::R64Sfloat: format_ = DXGI_FORMAT_R64_FLOAT; break;
			//case RHIFormat::R64G64Sfloat: format_ = DXGI_FORMAT_R64G64_FLOAT; break;
			//case RHIFormat::R64G64B64Sfloat: format_ = DXGI_FORMAT_R64G64B64_FLOAT; break;
			//case RHIFormat::R64G64B64A64Sfloat: format_ = DXGI_FORMAT_R64G64B64A64_FLOAT; break;
			case RHIFormat::D16Unorm: format_ = DXGI_FORMAT_D16_UNORM; break;
			case RHIFormat::D32Sfloat: format_ = DXGI_FORMAT_D32_FLOAT; break;
			//case RHIFormat::S8Uint: format_ = DXGI_FORMAT_S8_UINT; break;
			//case RHIFormat::D16UnormS8Uint: format_ = DXGI_FORMAT_D16_UNORM_S8_UINT; break;
			case RHIFormat::D24UnormS8Uint: format_ = DXGI_FORMAT_D24_UNORM_S8_UINT; break;
			case RHIFormat::D32SfloatS8Uint: format_ = DXGI_FORMAT_D32_FLOAT_S8X24_UINT; break;
			}
		}

		DXGI_FORMAT Get() { return format_; }

	private:
		DXGI_FORMAT format_{ DXGI_FORMAT_UNKNOWN };
	};

	class D3D12EnumQueueType final {
	public:
		D3D12EnumQueueType(RHIQueueType queueType) {
			switch (queueType) {
			case RHIQueueType::Graphics: queueType_ = D3D12_COMMAND_LIST_TYPE_DIRECT; break;
			case RHIQueueType::Compute: queueType_ = D3D12_COMMAND_LIST_TYPE_COMPUTE; break;
			case RHIQueueType::Transfer: queueType_ = D3D12_COMMAND_LIST_TYPE_COPY; break;
			case RHIQueueType::VideoDecode: queueType_ = D3D12_COMMAND_LIST_TYPE_VIDEO_DECODE; break;
			case RHIQueueType::VideoEncode: queueType_ = D3D12_COMMAND_LIST_TYPE_VIDEO_ENCODE; break;
			}
		}

		D3D12_COMMAND_LIST_TYPE Get() const { return queueType_; }

	private:
		D3D12_COMMAND_LIST_TYPE queueType_{ D3D12_COMMAND_LIST_TYPE_DIRECT };
	};

	class D3D12EnumAlphaMode final {
	public:
		D3D12EnumAlphaMode(RHIAlphaMode alphaMode) {
			switch (alphaMode) {
			case RHIAlphaMode::Undefined: alphaMode_ = DXGI_ALPHA_MODE_UNSPECIFIED; break;
			case RHIAlphaMode::Opaque: alphaMode_ = DXGI_ALPHA_MODE_IGNORE; break;
			case RHIAlphaMode::PreMultiplied: alphaMode_ = DXGI_ALPHA_MODE_PREMULTIPLIED; break;
			case RHIAlphaMode::PostMultiplied: alphaMode_ = DXGI_ALPHA_MODE_STRAIGHT; break;
			}
		}

		DXGI_ALPHA_MODE Get() const { return alphaMode_; }

	private:
		DXGI_ALPHA_MODE alphaMode_{ DXGI_ALPHA_MODE_UNSPECIFIED };
	};

	class D3D12EnumSampleCount final {
	public:
		D3D12EnumSampleCount(RHISampleCount sampleCount) {
			switch (sampleCount) {
			case RHISampleCount::Count1: sampleCount_ = 1; break;
			case RHISampleCount::Count2: sampleCount_ = 2; break;
			case RHISampleCount::Count4: sampleCount_ = 4; break;
			case RHISampleCount::Count8: sampleCount_ = 8; break;
			case RHISampleCount::Count16: sampleCount_ = 16; break;
			case RHISampleCount::Count32: sampleCount_ = 32; break;
			case RHISampleCount::Count64: sampleCount_ = 64; break;
			}
		}

		UINT Get() const { return sampleCount_; }
		
	private:
		UINT sampleCount_{ 1 };
	};

	class D3D12EnumResourceState final {
	public:
		D3D12EnumResourceState(RHIBufferUsage bufferUsage) {
			switch (bufferUsage) {
			case RHIBufferUsage::UniformBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case RHIBufferUsage::StorageBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case RHIBufferUsage::UniformTexelBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case RHIBufferUsage::StorageTexelBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case RHIBufferUsage::VertexBuffer: state = D3D12_RESOURCE_STATE_VERTEX_AND_CONSTANT_BUFFER; break;
			case RHIBufferUsage::IndexBuffer: state = D3D12_RESOURCE_STATE_INDEX_BUFFER; break;
			case RHIBufferUsage::TransferSrc: state = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
			case RHIBufferUsage::TransferDst: state = D3D12_RESOURCE_STATE_COPY_DEST; break;
			}
		}

		D3D12EnumResourceState(RHIImageUsage bufferUsage) {
			switch (bufferUsage) {
			case RHIImageUsage::ColorAttachment: state = D3D12_RESOURCE_STATE_RENDER_TARGET; break;
			case RHIImageUsage::DepthStencilAttachment: state = D3D12_RESOURCE_STATE_DEPTH_WRITE; break;
			case RHIImageUsage::Sampled: state = D3D12_RESOURCE_STATE_ALL_SHADER_RESOURCE; break;
			case RHIImageUsage::TransferSrc: state = D3D12_RESOURCE_STATE_COPY_SOURCE; break;
			case RHIImageUsage::TransferDst: state = D3D12_RESOURCE_STATE_COPY_DEST; break;
			}
		}

		D3D12_RESOURCE_STATES Get() const { return state; }

	private:
		D3D12_RESOURCE_STATES state{ D3D12_RESOURCE_STATE_COMMON };
	};

	class D3D12EnumUsage final {
	public:
		D3D12EnumUsage(RHIImageUsage imageUsage) {
			switch (imageUsage) {
			case RHIImageUsage::ColorAttachment: imageUsage_ = DXGI_USAGE_RENDER_TARGET_OUTPUT; break;
			case RHIImageUsage::DepthStencilAttachment: imageUsage_ = DXGI_USAGE_RENDER_TARGET_OUTPUT; break;
			case RHIImageUsage::Sampled: imageUsage_ = DXGI_USAGE_SHADER_INPUT; break;
			case RHIImageUsage::TransferSrc: imageUsage_ = DXGI_USAGE_READ_ONLY; break;
			case RHIImageUsage::TransferDst: imageUsage_ = DXGI_USAGE_UNORDERED_ACCESS; break;
			}
		}

		DXGI_USAGE Get() const { return imageUsage_; }

	private:
		DXGI_USAGE imageUsage_{ DXGI_USAGE_RENDER_TARGET_OUTPUT };
	};

	class D3D12EnumImageType final {
	public:
		D3D12EnumImageType(RHIImageType imageType) {
			switch (imageType) {
			case RHIImageType::Image1D: imageType_ = D3D12_RESOURCE_DIMENSION_TEXTURE1D; break;
			case RHIImageType::Image2D: imageType_ = D3D12_RESOURCE_DIMENSION_TEXTURE2D; break;
			case RHIImageType::Image3D: imageType_ = D3D12_RESOURCE_DIMENSION_TEXTURE3D; break;
			}
		}

		D3D12_RESOURCE_DIMENSION Get() const { return imageType_; }

	private:
		D3D12_RESOURCE_DIMENSION imageType_{ D3D12_RESOURCE_DIMENSION_TEXTURE1D };
	};

	class D3D12EnumRTVType final {
	public:
		D3D12EnumRTVType(RHIImageViewType imageViewType) {
			switch (imageViewType) {
			case RHIImageViewType::Image1D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE1D; break;
			case RHIImageViewType::Image2D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE2D; break;
			case RHIImageViewType::Image3D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE3D; break;
			case RHIImageViewType::ImageArray1D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE1DARRAY; break;
			case RHIImageViewType::ImageArray2D: imageViewType_ = D3D12_RTV_DIMENSION_TEXTURE2DARRAY; break;
			}
		}

		D3D12_RTV_DIMENSION Get() const { return imageViewType_; }

	private:
		D3D12_RTV_DIMENSION imageViewType_{ D3D12_RTV_DIMENSION_UNKNOWN };
	};

	class D3D12EnumSRVType final {
	public:
		D3D12EnumSRVType(RHIImageViewType imageViewType) {
			switch (imageViewType) {
			case RHIImageViewType::Image1D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE1D; break;
			case RHIImageViewType::Image2D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE2D; break;
			case RHIImageViewType::Image3D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE3D; break;
			case RHIImageViewType::ImageCube: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURECUBE; break;
			case RHIImageViewType::ImageArray1D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE1DARRAY; break;
			case RHIImageViewType::ImageArray2D: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURE2DARRAY; break;
			case RHIImageViewType::ImageArrayCube: imageViewType_ = D3D12_SRV_DIMENSION_TEXTURECUBEARRAY; break;
			}
		}

		D3D12_SRV_DIMENSION Get() const { return imageViewType_; }

	private:
		D3D12_SRV_DIMENSION imageViewType_{ D3D12_SRV_DIMENSION_UNKNOWN };
	};

	class D3D12EnumHeapType final {
	public:
		D3D12EnumHeapType(RHIDescriptorType descriptorType) {
			switch (descriptorType) {
			case RHIDescriptorType::Sampler: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER; break;
			case RHIDescriptorType::SampledImage: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case RHIDescriptorType::StorageImage: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case RHIDescriptorType::UniformBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case RHIDescriptorType::StorageBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case RHIDescriptorType::UniformTexelBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			case RHIDescriptorType::StorageTexelBuffer: heapType_ = D3D12_DESCRIPTOR_HEAP_TYPE_CBV_SRV_UAV; break;
			}
		}

		D3D12_DESCRIPTOR_HEAP_TYPE Get() const { return heapType_; }

	private:
		D3D12_DESCRIPTOR_HEAP_TYPE heapType_{ D3D12_DESCRIPTOR_HEAP_TYPE_SAMPLER };
	};

	class D3D12WrapperDevice final : public RHIDevice_T {
	public:
		D3D12WrapperDevice& SetDevice(ComPtr<ID3D12Device> device) { this->device = device; return *this; }
		D3D12WrapperDevice& SetQueues(std::vector<RHIQueue> queues) { this->queues = queues; return *this; }

		ComPtr<ID3D12Device> GetDevice() const { return device; }
		std::vector<RHIQueue> GetQueues() const { return queues; }

	private:
		ComPtr<ID3D12Device> device;
		std::vector<RHIQueue> queues;
	};

	class D3D12WrapperQueue final : public RHIQueue_T {
	public:
		D3D12WrapperQueue& SetQueueType(D3D12_COMMAND_LIST_TYPE queueType) { this->queueType = queueType; return *this; }
		D3D12WrapperQueue& SetQueue(ComPtr<ID3D12CommandQueue> queue) { this->queue = queue; return *this; }

		D3D12_COMMAND_LIST_TYPE GetQueueType() const { return queueType; }
		ComPtr<ID3D12CommandQueue> GetQueue() const { return queue; }

	private:
		D3D12_COMMAND_LIST_TYPE queueType;
		ComPtr<ID3D12CommandQueue> queue;
	};

	class D3D12WrapperSwapchain final : public RHISwapchain_T {
	public:
		D3D12WrapperSwapchain& SetSwapchain(ComPtr<IDXGISwapChain3> res) { resource = res; return *this; }
		ComPtr<IDXGISwapChain3> GetSwapchain() const { return resource; }

	private:
		ComPtr<IDXGISwapChain3> resource;
	};

	class D3D12WrapperCommandPool final : public RHICommandPool_T {
	public:
		D3D12WrapperCommandPool& SetQueueType(D3D12_COMMAND_LIST_TYPE queueType) { this->queueType = queueType; return *this; }
		D3D12WrapperCommandPool& SetCommandAllocator(ComPtr<ID3D12CommandAllocator> commandAllocator) { this->commandAllocator = commandAllocator; return *this; }
		D3D12WrapperCommandPool& SetCommandBuffers(std::vector<RHICommandBuffer> commandBuffers) { this->commandBuffers = commandBuffers; return *this; }

		D3D12_COMMAND_LIST_TYPE GetQueueType() const { return queueType; }
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const { return commandAllocator; }
		std::vector<RHICommandBuffer> GetCommandBuffers() const { return commandBuffers; }

	private:
		D3D12_COMMAND_LIST_TYPE queueType;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		std::vector<RHICommandBuffer> commandBuffers;
	};

	class D3D12WrapperCommandBuffer final : public RHICommandBuffer_T {
	public:
		D3D12WrapperCommandBuffer& SetGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList) { this->graphicsCommandList = graphicsCommandList; return *this; }
		ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const { return graphicsCommandList; }

	private:
		ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;
	};

	class D3D12WrapperBuffer final : public RHIBuffer_T {
	public:
		D3D12WrapperBuffer& SetResource(ComPtr<ID3D12Resource> resource) { this->resource = resource; return *this; }
		ComPtr<ID3D12Resource> GetResource() const { return resource; }

	private:
		ComPtr<ID3D12Resource> resource;
	};

	class D3D12WrapperBufferView final : public RHIBufferView_T {
	public:
		D3D12WrapperBufferView& SetBufferView(vk::BufferView bufferView) { this->bufferView = bufferView; return *this; }
		vk::BufferView GetBufferView() const { return bufferView; }

	private:
		 bufferView;
	};

	class D3D12WrapperImage final : public RHIImage_T {
	public:
		D3D12WrapperImage& SetResource(ComPtr<ID3D12Resource> resource) { this->resource = resource; return *this; }
		ComPtr<ID3D12Resource> GetResource() const { return resource; }

	private:
		ComPtr<ID3D12Resource> resource;
	};

	class D3D12WrapperImageView final : public RHIImageView_T {
	public:
		D3D12WrapperImageView& SetDecriptor(D3D12_CPU_DESCRIPTOR_HANDLE descriptor) { this->descriptor = descriptor; return *this; }
		D3D12_CPU_DESCRIPTOR_HANDLE GetDecriptor() const { return descriptor; }

	private:
		D3D12_CPU_DESCRIPTOR_HANDLE descriptor;
	};

	class D3D12WrapperDescriptorPool final : public RHIDescriptorPool_T {
	public:
		D3D12WrapperDescriptorPool& SetHeap(ComPtr<ID3D12Heap> heap) { this->heap = heap; return *this; }
		ComPtr<ID3D12Heap> GetHeap() const { return heap; }

	private:
		ComPtr<ID3D12Heap> heap;
	};

	class D3D12WrapperPipeline final : public RHIPipeline_T {
	public:
		D3D12WrapperPipeline& SetPipeline(ComPtr<ID3D12PipelineState> pipeline) { this->pipeline = pipeline; return *this; }
		ComPtr<ID3D12PipelineState> GetPipeline() const { return pipeline; }

	private:
		ComPtr<ID3D12PipelineState> pipeline;
	};



#endif // _DIRECT3D12