#pragma once

#include "vulkan_enum.hpp"

#ifdef _VULKAN

namespace engine {

	class VkWrapperInstance final : public rhi::Instance_T {
	public:
		void Init(const rhi::InstanceInitInfo&) override;
		void Destroy() override;

		std::vector<rhi::PhysicalDeviceInfo> GetPhysicalDeviceInfo() const override;

		rhi::Device CreateDevice(const rhi::DeviceCreateInfo&) const override;
		ShaderCompiler CreateShaderCompiler() const override;

	private:
		void CreateInstance(const rhi::InstanceInitInfo&);

	private:
		vk::Instance instance{ nullptr };

		struct {
			std::vector<const char*> instance;
			std::vector<const char*> device;
		}extensions;

		struct {
			const char* apiName{ nullptr };
			const char* applicationName{ nullptr };
			uint32_t applicationVersion{ 0 };
		}info;

		std::vector<const char*> validationLayers;

#ifdef _DEBUG
		vk::DebugUtilsMessengerEXT debugMessenger{ nullptr };
#endif // _DEBUG
	};

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
		//rhi::DescriptorPool CreateDescriptorPool(const rhi::DescriptorPoolCreateInfo&) const override;
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

	class VkWrapperQueue final : public rhi::Queue_T {
		friend class VkWrapperInstance;
		friend class VkWrapperDevice;
		vk::Device device{ nullptr };

	public:
		void SubmitCommandBuffers(uint32_t commandBufferCount, rhi::CommandBuffer* commandBuffers, rhi::Fence fence) override;
		void PresentSwapchain(rhi::Swapchain swapchain, uint32_t imageIndex) override;

	private:
		vk::Queue queue{ nullptr };
		uint32_t queueFamilyIndex{ 0 };
	};

	class VkWrapperSwapchain final : public rhi::Swapchain_T {
		friend class VkWrapperDevice;
		friend class VkWrapperQueue;
		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		rhi::Extent2D GetImageExtent() const override;
		std::vector<rhi::Image> GetImages() const override;
		uint32_t AcquireNextImage(rhi::Fence) override;

	private:
		vk::SurfaceKHR surface{ nullptr };
		vk::SwapchainKHR swapchain{ nullptr };
		std::vector<rhi::Image> images;
		rhi::Extent2D imageExtent;
		uint32_t currentImageIndex{ 0 };
	};

	class VkWrapperCommandPool final : public rhi::CommandPool_T {
		friend class VkWrapperDevice;
		vk::Device device{ nullptr };

	public:
		void Destroy() override;

		void Reset() override;
		
		std::vector<rhi::CommandBuffer> AllocateCommandBuffers(uint32_t bufferCount) override;

	private:
		vk::CommandPool commandPool{ nullptr };
		std::vector<rhi::CommandBuffer> commandBuffers;
	};

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
		void BindPipeline(const rhi::Pipeline&) override;
		void BindVertexBuffer(uint32_t bindingCount, rhi::Buffer* pBuffer, uint64_t* pOffsets) override;
		void BindIndexBuffer(rhi::Buffer& buffer, uint64_t offset, rhi::IndexType indexType) override;

		void Draw(uint32_t vertexCount, uint32_t instanceCount, uint32_t firstVertex, uint32_t firstInstance) const override;
		void DrawIndexed(uint32_t indexCount, uint32_t instanceCount, uint32_t firstVertex, int32_t vertexOffset, uint32_t firstInstance) const override;

	private:
		vk::CommandBuffer commandBuffer{ nullptr };
	};

	class VkWrapperBuffer final : public rhi::Buffer_T {
		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;
		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::Buffer buffer{ nullptr };
		vk::DeviceMemory memory{ nullptr };
	};

	class VkWrapperImage final : public rhi::Image_T {
		friend class VkWrapperDevice;
		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::Image image{ nullptr };
		vk::DeviceMemory memory{ nullptr };
	};

	class VkWrapperRenderPass final : public rhi::RenderPass_T {
		friend class VkWrapperDevice;
		friend class VkWrapperCommandBuffer;
		vk::Device device{ nullptr };

	public:
		void Destroy() override;

	private:
		vk::RenderPass renderPass{ nullptr };
	};

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
		uint32_t width, height, layers;
	};

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