#pragma once

#include "runtime/render/interface/rhi.hpp"
#include "d3d12_enum.hpp"

#ifdef _DIRECT3D12

namespace engine {

	class D3D12WrapperInstance final : public rhi::Instance_T {
	public:
		virtual void Init(const rhi::InstanceInitInfo&) = 0;
		virtual void Destroy() = 0;

		virtual std::vector<rhi::PhysicalDeviceInfo> GetPhysicalDeviceInfo() const = 0;

		virtual rhi::Device CreateDevice(const rhi::DeviceCreateInfo&) const = 0;
		virtual ShaderCompiler CreateShaderCompiler() const = 0;

	private:
		IDXGIFactory7* factory{ nullptr };

		struct {
			const char* apiName{ nullptr };
			const char* applicationName{ nullptr };
			uint32_t applicationVersion{ 0 };
		}info;

#ifdef _DEBUG
		ID3D12Debug* debugMessenger{ nullptr };
#endif // _DEBUG
	};

	class D3D12WrapperDevice final : public rhi::Device_T {
		friend class D3D12WrapperInstance;
		IDXGIFactory7* factory{ nullptr };

	public:
		virtual void Destroy() = 0;

		virtual rhi::Queue GetQueue(uint32_t queueIndex) const = 0;

		virtual rhi::Swapchain CreateSwapchain(const rhi::SwapchainCreateInfo&) const = 0;
		virtual CommandPool CreateCommandPool(const CommandPoolCreateInfo&) const = 0;
		virtual Buffer CreateBuffer(const BufferCreateInfo&) const = 0;
		virtual Image CreateImage(const ImageCreateInfo&) const = 0;
		//virtual DescriptorPool CreateDescriptorPool(const DescriptorPoolCreateInfo&) const = 0;
		virtual RenderPass CreateRenderPass(const RenderPassCreateInfo&) const = 0;
		virtual ShaderModule CreateShaderModule(const ShaderModuleCreateInfo&) const = 0;
		virtual Pipeline CreateGraphicsPipeline(const GraphicsPipelineCreateInfo&) const = 0;
		virtual Framebuffer CreateFramebuffer(const FramebufferCreateInfo&) const = 0;
		virtual Fence CreateFence(const FenceCreateInfo&) const = 0;

	private:
		ID3D12Device* device{ nullptr };
		std::vector<rhi::Queue> queues;
	};

	class D3D12WrapperQueue final : public rhi::Queue_T{
		friend class D3D12WrapperInstance;
		friend class D3D12WrapperDevice;

	public:
		virtual void SubmitCommandBuffers(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) = 0;
		virtual void PresentSwapchain(rhi::Swapchain swapchain, uint32_t imageIndex) = 0;

	private:
		ID3D12CommandQueue* queue{ nullptr };
		D3D12_COMMAND_LIST_TYPE commandListType{ D3D12_COMMAND_LIST_TYPE_DIRECT };
	};

	class D3D12WrapperSwapchain final : public rhi::Swapchain_T {
		friend class D3D12WrapperDevice;

	public:
		virtual void Destroy() = 0;

		virtual rhi::Extent2D GetImageExtent() const = 0;
		virtual std::vector<rhi::Image> GetImages() const = 0;
		virtual uint32_t AcquireNextImage(rhi::Fence) = 0;

	private:
		IDXGISwapChain3* swapchain{ nullptr };
	};

	class CommandPool_T {
	public:
		virtual void Destroy() = 0;

		virtual void Reset() = 0;

		virtual std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) = 0;
	};

	class CommandBuffer_T {
	public:
		virtual void Reset() = 0;
		virtual void Begin(const CommandBufferBeginInfo&) = 0;
		virtual void End() = 0;

		virtual void BeginRenderPass(const RenderPassBeginInfo&) = 0;
		virtual void EndRenderPass() = 0;
		virtual void BindPipeline(const Pipeline&) = 0;
		virtual void BindVertexBuffer(uint32_t bindingCount, Buffer* pBuffer, uint64_t* pOffsets) = 0;
		virtual void BindIndexBuffer(Buffer& buffer, uint64_t offset, IndexType indexType) = 0;

		virtual void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const = 0;
		virtual void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstVertex, int32_t vertexOffset, uint32_t firstInstance) const = 0;
	};

	class Buffer_T {
	public:
		virtual void Destroy() = 0;
	};

	class Image_T {
	public:
		virtual void Destroy() = 0;
	};

	class RenderPass_T {
	public:
		virtual void Destroy() = 0;
	};

	class ShaderModule_T {
	public:
		virtual void Destroy() = 0;

	};

	class Pipeline_T {
	public:
		virtual void Destroy() = 0;


	};

	class Framebuffer_T {
	public:
		virtual void Destroy() = 0;


	};

	class Fence_T {
	public:
		virtual void Destroy() = 0;

		virtual void Reset() = 0;
		virtual void Wait() = 0;

		virtual bool GetCurrentStatus() const = 0;
	};

	

}

#endif // _DIRECT3D12