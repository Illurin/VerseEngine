VkWrapperCommandPool& SetCommandBuffers(std::vector<RHICommandBuffer*> commandBuffers) { this->commandBuffers = commandBuffers; return *this; }
#pragma once

#ifdef _WIN32

#include "d3d12_util.hpp"

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

	class D3D12EnumImageUsage final {
	public:
		D3D12EnumImageUsage(RHIImageUsage imageUsage) {
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

	class D3D12WrapperDevice final : public RHIDevice {
	public:
		D3D12WrapperDevice& SetDevice(ComPtr<ID3D12Device> device) { this->device = device; return *this; }
		D3D12WrapperDevice& SetQueues(std::vector<RHIQueue*> queues) { this->queues = queues; return *this; }

		ComPtr<ID3D12Device> GetDevice() const { return device; }
		std::vector<RHIQueue*> GetQueues() const { return queues; }

	private:
		ComPtr<ID3D12Device> device;
		std::vector<RHIQueue*> queues;
	};

	class D3D12WrapperQueue final : public RHIQueue {
	public:
		D3D12WrapperQueue& SetQueueType(D3D12_COMMAND_LIST_TYPE queueType) { this->queueType = queueType; return *this; }
		D3D12WrapperQueue& SetQueue(ComPtr<ID3D12CommandQueue> queue) { this->queue = queue; return *this; }

		D3D12_COMMAND_LIST_TYPE GetQueueType() const { return queueType; }
		ComPtr<ID3D12CommandQueue> GetQueue() const { return queue; }

	private:
		D3D12_COMMAND_LIST_TYPE queueType;
		ComPtr<ID3D12CommandQueue> queue;
	};

	class D3D12WrapperSwapchain final : public RHISwapchain {
	public:
		D3D12WrapperSwapchain& SetSwapchain(ComPtr<IDXGISwapChain3> res) { resource = res; return *this; }
		ComPtr<IDXGISwapChain3> GetSwapchain() const { return resource; }

	private:
		ComPtr<IDXGISwapChain3> resource;
	};

	class D3D12WrapperCommandPool final : public RHICommandPool {
	public:
		D3D12WrapperCommandPool& SetQueueType(D3D12_COMMAND_LIST_TYPE queueType) { this->queueType = queueType; return *this; }
		D3D12WrapperCommandPool& SetCommandAllocator(ComPtr<ID3D12CommandAllocator> commandAllocator) { this->commandAllocator = commandAllocator; return *this; }
		D3D12WrapperCommandPool& SetCommandBuffers(std::vector<RHICommandBuffer*> commandBuffers) { this->commandBuffers = commandBuffers; return *this; }

		D3D12_COMMAND_LIST_TYPE GetQueueType() const { return queueType; }
		ComPtr<ID3D12CommandAllocator> GetCommandAllocator() const { return commandAllocator; }
		std::vector<RHICommandBuffer*> GetCommandBuffers() const { return commandBuffers; }

	private:
		D3D12_COMMAND_LIST_TYPE queueType;
		ComPtr<ID3D12CommandAllocator> commandAllocator;
		std::vector<RHICommandBuffer*> commandBuffers;
	};

	class D3D12WrapperCommandBuffer final : public RHICommandBuffer {
	public:
		D3D12WrapperCommandBuffer& SetGraphicsCommandList(ComPtr<ID3D12GraphicsCommandList> graphicsCommandList) { this->graphicsCommandList = graphicsCommandList; return *this; }
		ComPtr<ID3D12GraphicsCommandList> GetGraphicsCommandList() const { return graphicsCommandList; }

	private:
		ComPtr<ID3D12GraphicsCommandList> graphicsCommandList;
	};

}

#endif // _WIN32