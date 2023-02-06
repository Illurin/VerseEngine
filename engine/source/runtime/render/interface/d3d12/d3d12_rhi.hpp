#pragma once

#include "runtime/render/interface/rhi.hpp"
#include "d3d12_enum.hpp"

#ifdef _DIRECT3D12

namespace engine {

	//
	// Direct3D12 handle wrapper for RHI Instance
	//
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

	//
	// Direct3D12 handle wrapper for RHI Device
	//
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
		rhi::Sampler CreateSampler(const rhi::SamplerCreateInfo&) const override;
		rhi::DescriptorPool CreateDescriptorPool(const rhi::DescriptorPoolCreateInfo&) const override;
		rhi::DescriptorSetLayout CreateDescriptorSetLayout(const rhi::DescriptorSetLayoutCreateInfo&) const override;
		rhi::PipelineLayout CreatePipelineLayout(const rhi::PipelineLayoutCreateInfo&) const override;
		rhi::RenderPass CreateRenderPass(const rhi::RenderPassCreateInfo&) const override;
		rhi::ShaderModule CreateShaderModule(const rhi::ShaderModuleCreateInfo&) const override;
		rhi::Pipeline CreateGraphicsPipeline(const rhi::GraphicsPipelineCreateInfo&) const override;
		rhi::Framebuffer CreateFramebuffer(const rhi::FramebufferCreateInfo&) const override;
		rhi::Fence CreateFence(const rhi::FenceCreateInfo&) const override;

	private:
		ID3D12Device2* device{ nullptr };
		std::vector<rhi::Queue> queues;
		struct {
			UINT sampleQuality{ 0 };
		} feature;
	};

	//
	// Direct3D12 handle wrapper for RHI Queue
	//
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

	//
	// Direct3D12 handle wrapper for RHI Swapchain
	//
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

	//
	// Direct3D12 handle wrapper for RHI CommandPool
	//
	class D3D12WrapperCommandPool final : public rhi::CommandPool_T {

		friend class D3D12WrapperDevice;

		ID3D12Device* device{ nullptr };

	public:
		void Destroy() override;

		void Reset() override;
		void Free(uint32_t commandBufferCount, rhi::CommandBuffer* pCommandBuffers) override;

		std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) override;

	private:
		std::vector<rhi::CommandBuffer> commandBuffers;
		D3D12_COMMAND_LIST_TYPE commandListType{ D3D12_COMMAND_LIST_TYPE_DIRECT };
	};

	//
	// Direct3D12 handle wrapper for RHI CommandBuffer
	//
	class D3D12WrapperCommandBuffer final : public rhi::CommandBuffer_T{

		friend class D3D12WrapperCommandPool;
		friend class D3D12WrapperQueue;
		friend class D3D12WrapperPipeline;

	public:
		void Reset() override;
		void Begin(const rhi::CommandBufferBeginInfo&) override;
		void End() override;

		void BeginRenderPass(const rhi::RenderPassBeginInfo&) override;
		void EndRenderPass() override;
		void BindPipeline(const rhi::Pipeline&) override;
		void BindVertexBuffer(uint32_t firstBinding, uint32_t bindingCount, const rhi::Buffer* pBuffers) override;
		void BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) override;

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;
	
	private:
		void PrepareRenderingInfo();

	private:
		uint32_t poolIndex{ 0 };
		ID3D12CommandAllocator* commandAllocator{ nullptr };
		ID3D12GraphicsCommandList4* commandList{ nullptr };
		D3D12WrapperPipeline* pipeline{ nullptr };
		std::vector<D3D12_RESOURCE_BARRIER> renderPassResourceBarriers;

		struct {
			std::vector<D3D12_VERTEX_BUFFER_VIEW> vertexBufferViews;
			uint32_t firstBinding{ 0 };
			uint32_t bindingCount{ 0 };
			bool dirty{ false };
		} vertexBindingInfo;
	};

	//
	// Direct3D12 handle wrapper for RHI Buffer
	//
	class D3D12WrapperBuffer final : public rhi::Buffer_T {

		friend class D3D12WrapperDevice;
		friend class D3D12WrapperCommandBuffer;

	public:
		void Destroy() override;

		void* Map() override;
		void* Map(uint64_t offset, uint64_t size) override;
		void Unmap() override;

	private:
		ID3D12Resource* resource{ nullptr };
		UINT bufferSize{ 0 };
		D3D12_RANGE range{ 0, 0 };
	};

	//
	// Direct3D12 handle wrapper for RHI Image
	//
	class D3D12WrapperImage final : public rhi::Image_T{

		friend class D3D12WrapperDevice;

	public:
		void Destroy() override;

	private:
		ID3D12Resource* resource{ nullptr };
	};

	//
	// Direct3D12 handle wrapper for RHI Sampler
	//
	class D3D12WrapperSampler final : public rhi::Sampler_T {

		friend class D3D12WrapperDevice;

	public:
		void Destroy() override;

	private:
		rhi::SamplerCreateInfo samplerInfo{ {} };
	};

	//
	// Direct3D12 handle wrapper for RHI DescriptorPool
	//
	class D3D12WrapperDescriptorPool final : public rhi::DescriptorPool_T {

		friend class D3D12WrapperDevice;

	public:
		void Destroy() override;

		void Reset() override;
		void Free(uint32_t descriptorCount, rhi::DescriptorSet* pDescriptorSets) override;

		std::vector<rhi::DescriptorSet> AllocateDescriptorSets(const rhi::DescriptorSetAllocateInfo& info) override;

	private:
		ID3D12DescriptorHeap* cbvSrvUavHeap{ nullptr };
		ID3D12DescriptorHeap* samplerHeap{ nullptr };
		UINT cbvSrvUavHeapSize{ 0 };
		UINT samplerHeapSize{ 0 };
		std::queue<UINT> cbvSrvUavHeapSpareSpace;
		std::queue<UINT> samplerHeapSpareSpace;
		std::vector<rhi::DescriptorSet> descriptorSets;
	};

	//
	// Direct3D12 handle wrapper for RHI DescriptorSet
	//
	class D3D12WrapperDescriptorSet final : public rhi::DescriptorSet_T {

		friend class D3D12WrapperDevice;
		friend class D3D12WrapperDescriptorPool;

	public:
		void Write(uint32_t dstSet, uint32_t dstBinding, rhi::DescriptorType, rhi::Buffer) override;
		void Write(uint32_t dstSet, uint32_t dstBinding, rhi::DescriptorType, rhi::ImageViewInfo) override;

	private:
		uint32_t poolIndex{ 0 };
		std::vector<UINT> cbvSrvUavDescriptorIndices;
		std::vector<UINT> samplerDescriptorIndices;
	};

	//
	// Direct3D12 handle wrapper for RHI DescriptorSetLayout
	//
	class D3D12WrapperDescriptorSetLayout final : public rhi::DescriptorSetLayout_T {

		friend class D3D12WrapperDevice;
		friend class D3D12WrapperDescriptorPool;

	public:
		void Destroy() override;

	private:
		std::vector<rhi::DescriptorSetLayoutBinding> bindings;
	};

	//
	// Direct3D12 handle wrapper for RHI PipelineLayout
	//
	class D3D12WrapperPipelineLayout final : public rhi::PipelineLayout_T {

		friend class D3D12WrapperDevice;
		friend class D3D12WrapperPipeline;
		friend class D3D12WrapperCommandBuffer;

	public:
		void Destroy() override;

	private:
		ID3D12RootSignature* rootSignature{ nullptr };
	};

	//
	// Direct3D12 handle wrapper for RHI RenderPass
	//
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

	//
	// Direct3D12 handle wrapper for RHI ShaderModule
	//
	class D3D12WrapperShaderModule final : public rhi::ShaderModule_T {

		friend class D3D12WrapperDevice;
		friend class D3D12WrapperPipeline;

	public:
		void Destroy() override;

	private:
		std::vector<char> binarySource;
		rhi::ShaderStage shaderStage{ 0 };
	};

	//
	// Direct3D12 handle wrapper for RHI Pipeline
	//
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
			std::vector<uint32_t> bindingStride;
		} vertexInputInfo;

		struct {
			float minDepthBounds{ 0.0f };
			float maxDepthBounds{ 0.0f };
		} depthStencilInfo;
	};

	//
	// Direct3D12 handle wrapper for RHI Framebuffer
	//
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

	//
	// Direct3D12 handle wrapper for RHI Fence
	//
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