#pragma once

#include "runtime/render/interface/rhi.hpp"
#include "d3d12_enum.hpp"

#ifdef _DIRECT3D12

namespace engine {

	class D3D12WrapperInstance final : public rhi::Instance_T {
	public:
		void Init(const rhi::InstanceCreateInfo&) override;
		void Destroy() override;

		std::vector<rhi::PhysicalDeviceInfo> GetPhysicalDeviceInfo() const override;

		rhi::Device CreateDevice(const rhi::DeviceCreateInfo&) const override;
		ShaderCompiler CreateShaderCompiler() const override;

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
		virtual void Destroy() override;

		rhi::Queue GetQueue(uint32_t queueIndex) const override;

		rhi::Swapchain CreateSwapchain(const rhi::SwapchainCreateInfo&) const override;
		rhi::CommandPool CreateCommandPool(const rhi::CommandPoolCreateInfo&) const override;
		rhi::Buffer CreateBuffer(const rhi::BufferCreateInfo&) const override;
		rhi::Image CreateImage(const rhi::ImageCreateInfo&) const override;
		//rhi::DescriptorPool CreateDescriptorPool(const DescriptorPoolCreateInfo&) const override;
		rhi::RenderPass CreateRenderPass(const rhi::RenderPassCreateInfo&) const override;
		rhi::ShaderModule CreateShaderModule(const rhi::ShaderModuleCreateInfo&) const override;
		rhi::Pipeline CreateGraphicsPipeline(const rhi::GraphicsPipelineCreateInfo&) const override;
		rhi::Framebuffer CreateFramebuffer(const rhi::FramebufferCreateInfo&) const override;
		rhi::Fence CreateFence(const rhi::FenceCreateInfo&) const override;

	private:
		ID3D12Device2* device{ nullptr };
		std::vector<rhi::Queue> queues;
	};

	class D3D12WrapperQueue final : public rhi::Queue_T{
		friend class D3D12WrapperInstance;
		friend class D3D12WrapperDevice;

	public:
		void SubmitCommandBuffers(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) override;
		void PresentSwapchain(rhi::Swapchain swapchain, uint32_t imageIndex) override;

	private:
		ID3D12CommandQueue* queue{ nullptr };
		D3D12_COMMAND_LIST_TYPE commandListType{ D3D12_COMMAND_LIST_TYPE_DIRECT };
	};

	class D3D12WrapperSwapchain final : public rhi::Swapchain_T {
		friend class D3D12WrapperDevice;
		friend class D3D12WrapperQueue;

		ID3D12CommandQueue* queue{ nullptr };

	public:
		void Destroy() override;

		rhi::Extent2D GetImageExtent() const override;
		std::vector<rhi::Image> GetImages() const override;
		uint32_t AcquireNextImage(rhi::Fence) override;

	private:
		IDXGISwapChain3* swapchain{ nullptr };
		std::vector<rhi::Image> images;
		rhi::Extent2D imageExtent{ 0, 0 };
		uint32_t currentImageIndex{ 0 };
	};

	class D3D12WrapperCommandPool final : public rhi::CommandPool_T {
		friend class D3D12WrapperDevice;

		ID3D12Device* device{ nullptr };

	public:
		void Destroy() override;

		void Reset() override;

		std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) override;

	private:
		ID3D12CommandAllocator* commandAllocator{ nullptr };
		D3D12_COMMAND_LIST_TYPE commandListType{ D3D12_COMMAND_LIST_TYPE_DIRECT };
		std::vector<rhi::CommandBuffer> commandBuffers;
	};

	class D3D12WrapperCommandBuffer final : public rhi::CommandBuffer_T{
		friend class D3D12WrapperCommandPool;
		friend class D3D12WrapperQueue;

		ID3D12CommandAllocator* commandAllocator{ nullptr };

	public:
		void Reset() override;
		void Begin(const rhi::CommandBufferBeginInfo&) override;
		void End() override;

		void BeginRenderPass(const rhi::RenderPassBeginInfo&) override;
		void EndRenderPass() override;
		void BindPipeline(const rhi::Pipeline&) override;
		void BindVertexBuffer(uint32_t bindingCount, rhi::Buffer* pBuffer, uint64_t* pOffsets) override;
		void BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) override;

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) const override;
	
	private:
		ID3D12GraphicsCommandList4* commandList{ nullptr };
		std::vector<D3D12_RESOURCE_BARRIER> renderPassResourceBarriers;
	};

	class D3D12WrapperBuffer final : public rhi::Buffer_T {
	public:
		void Destroy() override;
	};

	class D3D12WrapperImage final : public rhi::Image_T{
		friend class D3D12WrapperDevice;

	public:
		void Destroy() override;

	private:
		ID3D12Resource* resource{ nullptr };
	};

	class D3D12WrapperRenderPass final : public rhi::RenderPass_T {
		friend class D3D12WrapperDevice;
		friend class D3D12WrapperCommandBuffer;

	public:
		void Destroy() override;

	private:
		uint32_t colorAttachmentCount{ 0 };
		rhi::AttachmentDescription* pColorAttachments{ nullptr };
		rhi::AttachmentDescription* pDepthStencilAttachment{ nullptr };
	};

	class D3D12WrapperShaderModule final : public rhi::ShaderModule_T {
		friend class D3D12WrapperDevice;
		friend class D3D12WrapperPipeline;

	public:
		void Destroy() override;

	private:
		std::vector<char> binarySource;
		rhi::ShaderStage shaderStage{ 0 };
	};

	class D3D12WrapperPipeline final : public rhi::Pipeline_T {
		friend class D3D12WrapperDevice;
		friend class D3D12WrapperCommandBuffer;

	public:
		void Destroy() override;

	private:
		ID3D12PipelineState* pipeline{ nullptr };
		std::vector<D3D12_RECT> scissors;
		std::vector<D3D12_VIEWPORT> viewports;
		D3D_PRIMITIVE_TOPOLOGY primitiveTopology{ D3D_PRIMITIVE_TOPOLOGY_UNDEFINED };
		struct {
			float minDepthBounds{ 0.0f };
			float maxDepthBounds{ 0.0f };
		} depthStencilInfo;
	};

	class D3D12WrapperFramebuffer final : public rhi::Framebuffer_T {
		friend class D3D12WrapperDevice;
		friend class D3D12WrapperCommandBuffer;

	public:
		void Destroy() override;

	private:
		std::vector<ID3D12Resource*> colorAttachments;
		ID3D12Resource* depthStencilAttachment{ nullptr };
		ID3D12DescriptorHeap* renderTargetHeap{ nullptr };
		ID3D12DescriptorHeap* depthStencilHeap{ nullptr };
		UINT rtvHandleIncrementSize{ 0 };
	};

	class D3D12WrapperFence final : public rhi::Fence_T {
		friend class D3D12WrapperDevice;
		friend class D3D12WrapperQueue;
		friend class D3D12WrapperSwapchain;

	public:
		void Destroy() override;

		void Reset() override;
		void Wait() override;

		bool GetCurrentStatus() const override;

	private:
		ID3D12Fence* fence{ nullptr };
	};

}

#endif // _DIRECT3D12