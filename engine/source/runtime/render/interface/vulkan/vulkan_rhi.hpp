#pragma once

#include "vulkan_enum.hpp"

#ifdef _VULKAN

namespace engine {

	//
	// Vulkan handle wrapper for RHI Instance
	//
	class VkWrapperInstance final : public rhi::Instance_T {
	public:
		void Init(const rhi::InstanceCreateInfo&) override;
		void Destroy() override;

		std::vector<rhi::PhysicalDeviceInfo> GetPhysicalDeviceInfo() const override;

		rhi::Device CreateDevice(const rhi::DeviceCreateInfo&) const override;
		ShaderCompiler CreateShaderCompiler() const override;

	private:
		void CreateInstance(const rhi::InstanceCreateInfo&);

	private:
		vk::Instance instance{ nullptr };

		struct {
			std::vector<const char*> instance;
			std::vector<const char*> device;
		} extensions;

		struct {
			const char* apiName{ nullptr };
			const char* applicationName{ nullptr };
			uint32_t applicationVersion{ 0 };
		} info;

		std::vector<const char*> validationLayers;

#ifdef _DEBUG
		vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
#endif // _DEBUG
	};

	//
	// Vulkan handle wrapper for RHI Device
	//
	class VkWrapperDevice final : public rhi::Device_T {

		friend class VkWrapperInstance;

		vk::Instance instance{ nullptr };

	public:
		void Destroy() override;
		
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
		vk::PhysicalDevice physicalDevice{ nullptr };
		vk::Device device{ nullptr };
		std::vector<rhi::Queue> queues;
	};

	//
	// Vulkan handle wrapper for RHI Queue
	//
	class VkWrapperQueue final : public rhi::Queue_T {

		friend class VkWrapperInstance;
		friend class VkWrapperDevice;

		vk::Device device{ nullptr };

	public:
		void Submit(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) override;
		void Present(rhi::Swapchain swapchain, uint32_t imageIndex) override;

		uint32_t AcquireNextImage(rhi::Swapchain) override;

	private:
		vk::Queue queue{ nullptr };
		uint32_t queueFamilyIndex{ 0 };
		vk::Semaphore semaphore{ nullptr };
		bool waitForSemaphore{ false };
	};

	//
	// Vulkan handle wrapper for RHI Swapchain
	//
	class VkWrapperSwapchain final : public rhi::Swapchain_T {

		friend class VkWrapperDevice;
		friend class VkWrapperQueue;

		vk::Instance instance{ nullptr };
		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		rhi::Extent2D GetImageExtent() const override;
		std::vector<rhi::Image> GetImages() const override;

	private:
		vk::SurfaceKHR surface{ nullptr };
		vk::SwapchainKHR swapchain{ nullptr };
		std::vector<rhi::Image> images;
		rhi::Extent2D imageExtent{};
		uint32_t currentImageIndex{ 0 };
	};

	//
	// Vulkan handle wrapper for RHI CommandPool
	//
	class VkWrapperCommandPool final : public rhi::CommandPool_T {

		friend class VkWrapperDevice;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		void Reset() override;
		void Free(uint32_t commandBufferCount, rhi::CommandBuffer* pCommandBuffers) override;
		
		std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) override;

	private:
		vk::CommandPool commandPool{ nullptr };
		std::vector<rhi::CommandBuffer> commandBuffers;
	};

	//
	// Vulkan handle wrapper for RHI CommandBuffer
	//
	class VkWrapperCommandBuffer final : public rhi::CommandBuffer_T {

		friend class VkWrapperCommandPool;
		friend class VkWrapperQueue;

		vk::CommandPool commandPool{ nullptr };

	public:
		void Reset() override;
		void Begin(const rhi::CommandBufferBeginInfo&) override;
		void End() override;

		void BeginRenderPass(const rhi::RenderPassBeginInfo&) override;
		void EndRenderPass() override;
		void BindPipeline(rhi::Pipeline&) override;
		void BindVertexBuffer(uint32_t firstBinding, uint32_t bindingCount, const rhi::Buffer* pBuffers) override;
		void BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) override;
		void BindDescriptorSets(rhi::PipelineType pipelineType, rhi::PipelineLayout layout, uint32_t firstSet, uint32_t descriptorSetCount, const rhi::DescriptorSet* pDescriptorSets) override;

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstIndex, int32_t vertexOffset, uint32_t firstInstance) override;

		void ResourceBarrier(const rhi::ImageMemoryBarrierInfo&) override;
		void CopyBufferToImage(const rhi::BufferCopyRegion&, const rhi::ImageCopyRegion&) override;

	private:
		uint32_t poolIndex{ 0 };
		vk::CommandBuffer commandBuffer{ nullptr };
	};

	//
	// Vulkan handle wrapper for RHI Buffer
	//
	class VkWrapperBuffer final : public rhi::Buffer_T {

		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;
		friend class VkWrapperDescriptorSet;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		void* Map() override;
		void* Map(uint64_t offset, uint64_t size) override;
		void Unmap() override;

	private:
		vk::Buffer buffer{ nullptr };
		vk::DeviceMemory memory{ nullptr };
	};

	//
	// Vulkan handle wrapper for RHI Image
	//
	class VkWrapperImage final : public rhi::Image_T {

		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;
		friend class VkWrapperDescriptorSet;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		rhi::Extent3D GetExtent() const;
		rhi::ImageCopyableFootprint GetCopyableFootprint() const;

	private:
		vk::Image image{ nullptr };
		vk::DeviceMemory memory{ nullptr };
		rhi::Extent3D extent{};
	};

	//
	// Vulkan handle wrapper for RHI Sampler
	//
	class VkWrapperSampler final : public rhi::Sampler_T {

		friend class VkWrapperDevice;
		friend class VkWrapperDescriptorSet;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::Sampler sampler{ nullptr };
	};

	//
	// Vulkan handle wrapper for RHI DescriptorPool
	//
	class VkWrapperDescriptorPool final : public rhi::DescriptorPool_T {

		friend class VkWrapperDevice;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		void Reset() override;
		void Free(uint32_t descriptorCount, rhi::DescriptorSet* pDescriptorSets) override;

		std::vector<rhi::DescriptorSet> AllocateDescriptorSets(const rhi::DescriptorSetAllocateInfo& info) override;

	private:
		vk::DescriptorPool descriptorPool{ nullptr };
		std::vector<rhi::DescriptorSet> descriptorSets;
	};

	//
	// Vulkan handle wrapper for RHI DescriptorSet
	//
	class VkWrapperDescriptorSet final : public rhi::DescriptorSet_T {

		friend class VkWrapperDevice;
		friend class VkWrapperDescriptorPool;
		friend class VkWrapperCommandBuffer;

		vk::Device device{ nullptr };

	public:
		void Write(uint32_t dstBinding, rhi::DescriptorType, rhi::Buffer) override;
		void Write(uint32_t dstBinding, rhi::DescriptorType, rhi::ImageViewInfo) override;
		void Write(uint32_t dstBinding, rhi::DescriptorType, rhi::Sampler) override;

	private:
		uint32_t poolIndex{ 0 };
		vk::DescriptorSet descriptorSet{ nullptr };
		vk::ImageView imageView{ nullptr };
	};

	//
	// Vulkan handle wrapper for RHI DescriptorSetLayout
	//
	class VkWrapperDescriptorSetLayout final : public rhi::DescriptorSetLayout_T {

		friend class VkWrapperDevice;
		friend class VkWrapperDescriptorPool;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::DescriptorSetLayout descriptorSetLayout{ nullptr };
		std::vector<vk::Sampler> samplers;
	};

	//
	// Vulkan handle wrapper for RHI PipelineLayout
	//
	class VkWrapperPipelineLayout final : public rhi::PipelineLayout_T {

		friend class VkWrapperDevice;
		friend class VkWrapperPipeline;
		friend class VkWrapperCommandBuffer;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::PipelineLayout pipelineLayout{ nullptr };
	};

	//
	// Vulkan handle wrapper for RHI RenderPass
	//
	class VkWrapperRenderPass final : public rhi::RenderPass_T {

		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::RenderPass renderPass{ nullptr };
	};

	//
	// Vulkan handle wrapper for RHI ShaderModule
	//
	class VkWrapperShaderModule final : public rhi::ShaderModule_T {

		friend class VkWrapperDevice;
		friend class VkWrapperPipeline;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::ShaderModule shaderModule{ nullptr };
		const char* entryPoint{ nullptr };
		rhi::ShaderStage shaderStage{ 0 };
	};

	//
	// Vulkan handle wrapper for RHI Pipeline
	//
	class VkWrapperPipeline final : public rhi::Pipeline_T {

		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::Pipeline pipeline{ nullptr };
		rhi::PipelineType pipelineType{ 0 };
	};

	//
	// Vulkan handle wrapper for RHI Framebuffer
	//
	class VkWrapperFramebuffer final : public rhi::Framebuffer_T {

		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::Framebuffer framebuffer{ nullptr };
		std::vector<vk::ImageView> colorAttachments;
		vk::ImageView depthStencilAttachment{ nullptr };
		uint32_t width{ 0 }, height{ 0 }, layers{ 0 };
	};

	//
	// Vulkan handle wrapper for RHI Fence
	//
	class VkWrapperFence final : public rhi::Fence_T {

		friend class VkWrapperDevice;
		friend class VkWrapperQueue;
		friend class VkWrapperSwapchain;

		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		void Reset() override;
		void Wait() override;

		bool GetCurrentStatus() const override;

	private:
		vk::Fence fence{ nullptr };
	};
	
}

#endif // _VULKAN